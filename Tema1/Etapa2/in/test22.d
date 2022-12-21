import std.conv;
import std.digest;
import std.digest.sha;
import std.stdio;

import vibe.d;
import vibe.web.auth;

import db_conn;

import dauth : makeHash, toPassword, parseHash;

static ubyte[] mySalt = [23, 4, 2];

static struct AuthInfo
{
@safe:
    string userEmail;
}

@path("api/v1")
@requiresAuth
interface VirusTotalAPIRoot
{
    // Users management
    @noAuth
    @method(HTTPMethod.POST)
    @path("signup")
    Json addUser(string userEmail, string username, string password, string name = "", string desc = "");

    @noAuth
    @method(HTTPMethod.POST)
    @path("login")
    Json authUser(string userEmail, string password);

    @anyAuth
    @method(HTTPMethod.POST)
    @path("delete_user")
    Json deleteUser(string userEmail);

    // URLs management
    @anyAuth
    @method(HTTPMethod.POST)
    @path("add_url") // the path could also be "/url/add", thus defining the url "namespace" in the URL
    Json addUrl(string userEmail, string urlAddress);

    @noAuth
    @method(HTTPMethod.GET)
    @path("url_info")
    Json getUrlInfo(string urlAddress);

    @noAuth
    @method(HTTPMethod.GET)
    @path ("user_urls")
    Json getUserUrls(string userEmail);

    @anyAuth
    @method(HTTPMethod.POST)
    @path("delete_url")
    Json deleteUrl(string userEmail, string urlAddress);

    // Files management
    @anyAuth
    @method(HTTPMethod.POST)
    @path("add_file")
    Json addFile(string userEmail, immutable ubyte[] binData, string fileName);

    @noAuth
    @method(HTTPMethod.GET)
    @path("file_info")
    Json getFileInfo(string fileSHA512Digest);

    @noAuth
    @method(HTTPMethod.GET)
    @path("user_files")
    Json getUserFiles(string userEmail);

    @anyAuth
    @method(HTTPMethod.POST)
    @path("delete_file")
    Json deleteFile(string userEmail, string fileSHA512Digest);
}

class VirusTotalAPI : VirusTotalAPIRoot
{
    this(DBConnection dbClient)
    {
        this.dbClient = dbClient;
    }

    @noRoute AuthInfo authenticate(scope HTTPServerRequest req, scope HTTPServerResponse res)
    {
        // If "userEmail" is not present, an error 500 (ISE) will be returned
        string userEmail = req.json["userEmail"].get!string;
        string userAccessToken = dbClient.getUserAccessToken(userEmail);
        // Use headers.get to check if key exists
        string headerAccessToken = req.headers.get("AccessToken");
        if (headerAccessToken && headerAccessToken == userAccessToken)
            return AuthInfo(userEmail);
        throw new HTTPStatusException(HTTPStatus.unauthorized);
    }

override:

    Json addUser(string userEmail, string username, string password, string name = "", string desc = "")
    {
        // TODO
        if (password == null)
            throw new HTTPStatusException(HTTPStatus.badRequest, "null password");

        DBConnection.UserRet resp = dbClient.addUser(userEmail, username, makeHash(toPassword(password.to!(char[])), mySalt).toString(), name, desc);

        if (resp == DBConnection.UserRet.ERR_INVALID_EMAIL)
            throw new HTTPStatusException(HTTPStatus.badRequest, "invalid email");
        if (resp == DBConnection.UserRet.ERR_USER_EXISTS)
            throw new HTTPStatusException(HTTPStatus.unauthorized, "existing user");
        if (resp == DBConnection.UserRet.OK)
            return serializeToJson(["response" : "ok"]);

        throw new HTTPStatusException(HTTPStatus.internalServerError, "[Internal Server Error] user action not defined");
    }

    Json authUser(string userEmail, string password)
    {
        // TODO
        if (password == null)
            throw new HTTPStatusException(HTTPStatus.badRequest, "null password");

        DBConnection.UserRet resp = dbClient.authUser(userEmail, makeHash(toPassword(password.to!(char[])), mySalt).toString());

        if (resp == DBConnection.UserRet.ERR_INVALID_EMAIL)
            throw new HTTPStatusException(HTTPStatus.badRequest, "invalid email");
        
        if (resp == DBConnection.UserRet.ERR_WRONG_PASS)
            throw new HTTPStatusException(HTTPStatus.unauthorized, "wrong pass : " ~ makeHash(toPassword(password.to!(char[])), mySalt).toString());
        if (resp == DBConnection.UserRet.ERR_WRONG_USER)
            throw new HTTPStatusException(HTTPStatus.unauthorized, "wrong user");

        if (resp == DBConnection.UserRet.OK)
            return serializeToJson(["AccessToken" : dbClient.generateUserAccessToken(userEmail)]);

        throw new HTTPStatusException(HTTPStatus.internalServerError, "[Internal Server Error] user action not defined");
    }

    Json deleteUser(string userEmail)
    {
        // TODO
        DBConnection.UserRet resp = dbClient.deleteUser(userEmail);

        if (resp == DBConnection.UserRet.ERR_INVALID_EMAIL)
            throw new HTTPStatusException(HTTPStatus.badRequest, "invalid email");
        if (resp == DBConnection.UserRet.OK)
            return serializeToJson(["response" : "ok"]);

        throw new HTTPStatusException(HTTPStatus.internalServerError, "[Internal Server Error] user action not defined");
    }

    // URLs management

    Json addUrl(string userEmail, string urlAddress)
    {
        // TODO
        if (urlAddress == null)
            throw new HTTPStatusException(HTTPStatus.badRequest, "empty url");
        
        DBConnection.UrlRet resp = dbClient.addUrl(userEmail, urlAddress);

        if (resp == DBConnection.UrlRet.URL_EXISTS || resp == DBConnection.UrlRet.OK)
            return serializeToJson(["response" : "ok"]);

        throw new HTTPStatusException(HTTPStatus.internalServerError, "[Internal Server Error] user action not defined");
    }

    Json deleteUrl(string userEmail, string urlAddress)
    {
        // TODO
        if (urlAddress == null)
            throw new HTTPStatusException(HTTPStatus.badRequest, "empty url");
        
        dbClient.deleteUrl(userEmail, urlAddress);

        return serializeToJson(["response" : "ok"]);
    }

    Json getUrlInfo(string urlAddress)
    {
        // TODO
        auto url = dbClient.getUrl(urlAddress);
        if (url.isNull)
            throw new HTTPStatusException(HTTPStatus.notFound, "url not found");
        
        return serializeToJson(url.get);
    }

    Json getUserUrls(string userEmail)
    {
        // TODO
        return serializeToJson(dbClient.getUrls(userEmail));
    }

    // Files management

    Json addFile(string userEmail, immutable ubyte[] binData, string fileName)
    {
        // TODO
        DBConnection.FileRet resp = dbClient.addFile(userEmail, binData, fileName);

        if (resp == DBConnection.FileRet.ERR_EMPTY_FILE)
            throw new HTTPStatusException(HTTPStatus.badRequest, "empty file");
        
        if (resp == DBConnection.FileRet.OK || resp == DBConnection.FileRet.FILE_EXISTS)
            return serializeToJson(["response" : "ok"]);

        throw new HTTPStatusException(HTTPStatus.internalServerError, "[Internal Server Error] user action not defined");
    }

    Json getFileInfo(string fileSHA512Digest)
    {
        // TODO
        auto respFile = dbClient.getFile(fileSHA512Digest);

        if (respFile.isNull)
            throw new HTTPStatusException(HTTPStatus.notFound, "file not found");
        
        DBConnection.File file = respFile.get;
        
        return serializeToJson(file);
    }

    Json getUserFiles(string userEmail)
    {
        // TODO
        return serializeToJson(dbClient.getFiles(userEmail));
    }

    Json deleteFile(string userEmail, string fileSHA512Digest)
    {
        // TODO
        if (fileSHA512Digest == null)
            throw new HTTPStatusException(HTTPStatus.badRequest, "empty digest");
        
        dbClient.deleteFile(userEmail, fileSHA512Digest);
        return serializeToJson(["response" : "ok"]);
    }

private:
    DBConnection dbClient;
}

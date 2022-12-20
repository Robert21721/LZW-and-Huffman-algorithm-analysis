#include <bits/stdc++.h>
using namespace std;
# define SIZE 20

vector<int> encoding(char *file_in) {
	FILE *fin = fopen(file_in, "rb");

	fseek(fin, 0, SEEK_END);
	long long dim = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	unordered_map<string, int> table;
	for (int i = 0; i <= 255; i++) {
		string ch = "";
		ch += char(i);
		table[ch] = i;
	}

	char caracter;
	fread(&caracter, sizeof(char), 1, fin);


	string p = "", c = "";
	p += caracter;
	int code = 256;
	vector<int> output_code;


	for (int i = 0; i < dim; i++) {
		if (i != dim - 1) {
			fread(&caracter, sizeof(char), 1, fin);
			c += caracter;
		}

		if (table.find(p + c) != table.end()) {
			p = p + c;

		} else {
			//cout << p << "\t" << table[p] << "\t\t" << p + c << "\t" << code << endl;


			output_code.push_back(table[p]);
			table[p + c] = code;
			code++;
			p = c;
		}
		c = "";
	}
	// cout << p << "\t" << table[p] << endl;
	output_code.push_back(table[p]);
	output_code.push_back((1 << (SIZE)) - 1);
	return output_code;
}

void write_to_compressed_file(vector<int> output_code, char* file_out) {
	unsigned int buff = 0;
	int pos = 8 * sizeof(unsigned int);

	FILE *fout = fopen(file_out, "wb");

	for (int i = 0; i < output_code.size(); i++) {
		if (pos < SIZE) {
			buff = buff + (output_code[i] >> (SIZE - pos));
			// printf("buff1 dupa update %d\n", buff);

			fwrite(&buff, sizeof(int), 1, fout);

			buff = output_code[i] << (8 * sizeof(unsigned int) - SIZE + pos);
			pos = 8 * sizeof(unsigned int) - SIZE + pos;
			// printf("buff2 dupa update %d\n", buff);
		} else {
			buff = buff + (output_code[i] << (pos - SIZE));
			pos = pos - SIZE;
			// printf("buff normal %d\n", buff);
		}
	}

	fwrite(&buff, sizeof(int), 1, fout);
	fclose(fout);
}

vector<int> read_from_compress_file(char *file_in) {
	FILE *fin = fopen(file_in, "rb");

	vector<int> output_code;

	unsigned int mask = 0;
	int pos = 8 * sizeof(unsigned int);

	for (int i = 0; i < SIZE; i++) {
		mask = (mask << 1) + 1;
	}

	unsigned int buff;
	fread(&buff, sizeof(unsigned int), 1, fin);

	while (true) {
		int aux = 0;

		if (pos < SIZE) {
			aux = buff & (mask >> (SIZE - pos));
			aux = aux << (SIZE - pos);

			fread(&buff, sizeof(unsigned int), 1, fin);

			// aux = aux + (buff & (mask << (pos + SIZE - 8 * sizeof(unsigned int))) >> (pos + SIZE - 8 * sizeof(unsigned int)));
			int shift_val = 8 * sizeof(unsigned int) - SIZE + pos;
			aux = aux + ((buff & (mask << shift_val)) >> shift_val);
			pos = 8 * sizeof(unsigned int) - SIZE + pos;
			
		} else {
			aux = (buff & (mask << (pos - SIZE))) >> (pos - SIZE);
			pos = pos - SIZE;
		}

		// printf("%d\n", aux);

		if (aux != (1 << (SIZE)) - 1) {
			output_code.push_back(aux);
		} else {
			break;
		}
	}

	return output_code;
}

void decoding(vector<int> op, char *file_name) {
	// cout << "\nDecoding\n";
	unordered_map<int, string> table;


	for (int i = 0; i <= 255; i++) {
		string ch = "";
		ch += char(i);
		table[i] = ch;
	}

	FILE *fout = fopen(file_name, "wb");

	int old = op[0], n;
	string s = table[old];
	string c = "";
	c += s[0];

	// cout << s;
	for (int i = 0; i < s.size(); i++) {
		fwrite(&s[i], sizeof(char), 1, fout);
	}

	int count = 256;
	for (int i = 0; i < op.size() - 1; i++) {
		n = op[i + 1];

		if (table.find(n) == table.end()) {
			s = table[old];
			s = s + c;
		} else {
			s = table[n];
		}

		// cout << s;
		for (int i = 0; i < s.size(); i++) {
			fwrite(&s[i], sizeof(char), 1, fout);
		}

		c = "";
		c += s[0];
		table[count] = table[old] + c;
		count++;
		old = n;
	}
    // cout<<endl;
}
int main(int argc, char *argv[])
{
	if (argc < 4 || argc > 5) {
		printf("nr invalid de argumente\n");
		return -1;
	} 
	// string s = "WYS*WYGWYS*WYSWYSG";
	if (strcmp(argv[1], "-c") == 0) {
        vector<int> output_code = encoding(argv[2]);
		write_to_compressed_file(output_code, argv[3]);
    }


    if (strcmp(argv[1], "-d") == 0) {
        vector<int> output_code_decod = read_from_compress_file(argv[2]);
		decoding(output_code_decod, argv[3]);


    }
}

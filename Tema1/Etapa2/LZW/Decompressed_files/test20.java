import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

// DIAGRAM CLASSES
class DiagramComponent {
    private String text = "text";
    private String color = "WHITE";
    private int height = 40;
    private int width = 100;

    @Override
    public String toString() {
        return "[" +
                "text='" + text + '\'' +
                ", color='" + color + '\'' +
                ", height=" + height +
                ", weight=" + width +
                ", connectedComponents=" + connectedComponents +
                ']';
    }

    private List<String> connectedComponents = new ArrayList<>();

    public String getText() {
        return text;
    }

    public void setText(String text) {
        this.text = text;
    }

    public String getColor() {
        return color;
    }

    public void setColor(String color) {
        this.color = color;
    }

    public int getHeight() {
        return height;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public int getWidth() {
        return width;
    }

    public void setWidth(int width) {
        this.width = width;
    }

    public void connectTo(String componentId) {
        connectedComponents.add(componentId);
    }

    public void removeConnection(String componentId) {
        connectedComponents.remove(componentId);
    }
}


class DiagramCanvas {
    private List<DiagramComponent> components = new ArrayList<>();

    public void addComponent(DiagramComponent diagramComponent) {
        components.add(diagramComponent);
    }

    public void removeComponent(DiagramComponent diagramComponent) {
        components.remove(diagramComponent);
    }

    public DiagramComponent getComponent(int id) {
        return components.get(id);
    }

    public void show() {
        System.out.println("Diagram:");
        components.forEach(System.out::println);
    }
}


// COMMAND CLASSES
enum CommandType {
    DRAW_RECTANGLE("draw rectangle"),
    CHANGE_COLOR("change color"),
    RESIZE("resize"),
    CONNECT("connect"),
    CHANGE_TEXT("change text");

    public final String text;

    CommandType(String text) {
        this.text = text;
    }

    public static CommandType fromString(String text) {
        for (CommandType commandType : CommandType.values()) {
            if (commandType.text.equalsIgnoreCase(text)) {
                return commandType;
            }
        }
        return null;
    }
}

interface DrawCommand {
    void execute();
    void undo();
}

class Invoker {
    LinkedList<DrawCommand> executedQueue;
    LinkedList<DrawCommand> undoneQueue;

    /**
     * Clear up all the used resources, start fresh :D
     */
    public void restart() {
        // TODO
        executedQueue = new LinkedList<DrawCommand>();
        undoneQueue = new LinkedList<DrawCommand>();
    }

    /**
     * Executes a given command
     * @param command
     */
    public void execute(DrawCommand command) {
        command.execute();
        executedQueue.add(command);
    }

    /**
     * Undo the latest command
     */
    public void undo() {
        // TODO
        // Hint: use data structures to keep track of commands
        // Do not use the java.util.Stack, its implementation is based on vector which is inefficient and deprecated.
        // Use a class that implements the Deque interface, e.g. LinkedList https://docs.oracle.com/en/java/javase/11/docs/api/java.base/java/util/Deque.html
        DrawCommand command = executedQueue.getLast();
        executedQueue.removeLast();
        command.undo();
        undoneQueue.add(command);
    }

    /**
     * Redo command previously undone. Cannot perform a redo after an execute, only after at least one undo.
     */
    public void redo() {
        // TODO
        DrawCommand command = undoneQueue.getLast();
        undoneQueue.removeLast();
        command.execute();
    }
}

class DrawRectangleCommand implements DrawCommand {
    private DiagramCanvas diagramCanvas;
    private DiagramComponent diagramComponent;

    public DrawRectangleCommand(DiagramCanvas diagramCanvas) {
        this.diagramCanvas = diagramCanvas;
        this.diagramComponent = new DiagramComponent();
    }

    @Override
    public void execute() {
        diagramCanvas.addComponent(diagramComponent);
    }

    @Override
    public void undo() {
        diagramCanvas.removeComponent(diagramComponent);
    }
}

class ResizeCommand implements DrawCommand {
    private DiagramComponent component;

    private int newHeight;
    private int oldHeight;
    private int newWidth;
    private int oldWidth;

    public ResizeCommand(DiagramCanvas diagramCanvas, int index, double percent) {
        this.component = diagramCanvas.getComponent(index);
        this.oldWidth = component.getWidth();
        this.oldHeight = component.getHeight();
        this.newHeight = (int)(oldHeight * percent / 100.0);
        this.newWidth = (int)(oldWidth * percent / 100.0);
    }

    @Override
    public void execute() {
        component.setHeight(newHeight);
        component.setWidth(newWidth);
    }

    @Override
    public void undo() {
        component.setHeight(oldHeight);
        component.setWidth(oldWidth);
    }
}

class ChangeColorCommand implements DrawCommand {
    private DiagramComponent diagramComponent;
    private String newColor;
    private String oldColor;

    public ChangeColorCommand(DiagramCanvas diagramCanvas, int index, String color) {
        this.diagramComponent = diagramCanvas.getComponent(index);
        this.oldColor = diagramComponent.getColor();
        this.newColor = color;
    }

    @Override
    public void execute() {
        this.diagramComponent.setColor(newColor);
    }

    @Override
    public void undo() {
        this.diagramComponent.setColor(oldColor);
    }
}

class ChangeTextCommand implements DrawCommand {
    private DiagramComponent diagramComponent;
    private String newText;
    private String oldText;

    public ChangeTextCommand(DiagramCanvas diagramCanvas, int index, String text) {
        this.diagramComponent = diagramCanvas.getComponent(index);
        this.oldText = diagramComponent.getText();
        this.newText = text;
    }

    @Override
    public void execute() {
        this.diagramComponent.setText(newText);
    }

    @Override
    public void undo() {
        this.diagramComponent.setText(oldText);
    }
}

class ConnectComponentsCommand implements DrawCommand {
    private DiagramCanvas diagramCanvas;
    private DiagramComponent diagramComponent1;
    private String id1;
    private DiagramComponent diagramComponent2;
    private String id2;

    public ConnectComponentsCommand(DiagramCanvas diagramCanvas, String id1, String id2) {
        this.diagramCanvas = diagramCanvas;
        this.diagramComponent1 = diagramCanvas.getComponent(Integer.parseInt(id1));
        this.diagramComponent2 = diagramCanvas.getComponent(Integer.parseInt(id2));
        this.id1 = id1;
        this.id2 = id2;
    }

    @Override
    public void execute() {
        diagramComponent1.connectTo(id2);
        diagramComponent2.connectTo(id1);
    }

    @Override
    public void undo() {
        diagramComponent1.removeConnection(id2);
        diagramComponent2.removeConnection(id1);
    }
}

class Client {

    private Invoker invoker;
    private DiagramCanvas diagramCanvas;

    Client() {
        invoker = new Invoker();
        diagramCanvas = new DiagramCanvas();
    }

    public void showDiagram() {
        diagramCanvas.show();
    }

    public void newDiagram() {
        diagramCanvas = new DiagramCanvas();
        invoker.restart();
    }

    public void executeAction(String commandName, String ...args) {
        DrawCommand command;
        try {
            CommandType commandType = CommandType.fromString(commandName);
            if (commandType == null) {
                throw new IllegalArgumentException();
            }
            command = getCommand(commandType, args);

        } catch(IllegalArgumentException ex) {
            System.out.println("Invalid command: " + commandName);
            System.out.println("Available commands:");
            for (CommandType type : CommandType.values()) {
                System.out.println("\t- " + type.text);
            }
            return;
        }

        invoker.execute(command);
    }

    private DrawCommand getCommand(CommandType type, String ...args) throws IllegalArgumentException {
        try {
             switch (type) {
                 case DRAW_RECTANGLE:
                    return new DrawRectangleCommand(diagramCanvas);
                 case RESIZE:
                     return new ResizeCommand(diagramCanvas, Integer.parseInt(args[0]), Double.parseDouble(args[1]));
                 case CONNECT:
                     return new ConnectComponentsCommand(diagramCanvas, args[0], args[1]);
                 case CHANGE_TEXT:
                     return new ChangeTextCommand(diagramCanvas, Integer.parseInt(args[0]), args[1]);
                 case CHANGE_COLOR:
                     return new ChangeColorCommand(diagramCanvas, Integer.parseInt(args[0]), args[1]);
            };
        } catch (NumberFormatException ex) {
            throw new IllegalArgumentException();
        }

        return null;
    }

    public void undo(){
        invoker.undo();
    }

    public void redo() {
        invoker.redo();
    }
}

public class Command {
    private static String spacerSymbols = new String(new char[40]).replace("\0", "-");

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int taskNum = scanner.nextInt();

        Client client = new Client();

        switch(taskNum) {
            case 1:
                printOutputSpacerFor("testDraw");
                testDraw(client);

                printOutputSpacerFor("testTextAndColor");
                testTextAndColor(client);

                printOutputSpacerFor("testResize");
                testResize(client);

                printOutputSpacerFor("testConnect");
                testConnect(client);

                printOutputSpacerFor("testAllCommands");
                testAllCommands(client);

                break;
            case 2:
                printOutputSpacerFor("testSimpleUndoRedo");
                testSimpleUndoRedo(client);

                printOutputSpacerFor("testComplexUndoRedo");
                testComplexUndoRedo(client);

                break;
        }

    }

    private static void printOutputSpacerFor(String test) {
        System.out.println(spacerSymbols + test + spacerSymbols);
    }

    private static void testDraw(Client client) {
        client.newDiagram();

        client.executeAction("draw rectangle");
        client.executeAction("draw rectangle");
        client.executeAction("draw rectangle");
        client.executeAction("draw rectangle");
        client.executeAction("draw rectangle");

        client.showDiagram();
    }

    private static void testTextAndColor(Client client) {
        testDraw(client);
        client.executeAction("change color", "0", "RED");
        client.executeAction("change color", "1", "BLUE");
        client.executeAction("draw rectangle");
        client.executeAction("change text", "0", "MyClass1");
        client.executeAction("change text", "5", "MyClass2");
        client.showDiagram();
    }

    private static void testConnect(Client client) {
        testDraw(client);
        client.executeAction("connect", "0", "1");
        client.executeAction("connect", "3", "2");
        client.executeAction("connect", "1", "4");
        client.executeAction("draw rectangle");
        client.showDiagram();
    }

    private static void testResize(Client client) {
        testDraw(client);
        client.executeAction("resize", "0", "10");
        client.executeAction("resize", "2", "50");
        client.executeAction("resize", "4", "25");
        client.showDiagram();
    }

    private static void testAllCommands(Client client) {
        client.newDiagram();

        client.executeAction("draw rectangle");
        client.executeAction("draw rectangle");
        client.executeAction("draw rectangle");
        client.executeAction("resize", "0", "10");
        client.executeAction("change color", "2", "PINK");
        client.executeAction("connect", "2", "1");
        client.executeAction("draw rectangle");
        client.executeAction("change text", "1", "Class1");
        client.executeAction("change text", "3", "Class2");

        client.showDiagram();
    }

    private static void testSimpleUndoRedo(Client client) {
        client.newDiagram();

        client.executeAction("draw rectangle");
        client.executeAction("change color", "0", "ORANGE");
        client.showDiagram();

        client.executeAction("draw rectangle");
        client.showDiagram();

        client.undo();
        client.showDiagram();

        client.redo();
        client.showDiagram();
    }

    private static void testComplexUndoRedo(Client client) {
        client.executeAction("draw rectangle");
        client.executeAction("draw rectangle");
        client.executeAction("draw rectangle");
        client.executeAction("resize", "0", "10");
        client.executeAction("change color", "2", "PINK");
        client.executeAction("connect", "2", "1");
        client.executeAction("draw rectangle");
        client.executeAction("change text", "1", "Class1");
        client.executeAction("change color", "3", "RED");
        client.showDiagram();
        client.undo();
        client.undo();
        client.showDiagram();

        client.undo();
        client.undo();
        client.showDiagram();

        client.redo();
        client.redo();
        client.executeAction("resize", "2", "50");
        client.showDiagram();
    }
}
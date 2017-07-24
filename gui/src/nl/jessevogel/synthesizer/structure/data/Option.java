package nl.jessevogel.synthesizer.structure.data;

public class Option {
    public enum Type { Boolean, Integer, Float }

    public String label;
    public Type type;
    public String value;
    public String description;

    public Option() {}

    public Option(Option option) {
        label = option.label;
        type = option.type;
        value = option.value;
        description = option.description;
    }

    public static Type getType(String type) throws Exception {
        switch(type) {
            case "bool":
            case "boolean":
                return Type.Boolean;

            case "float":
            case "double":
                return Type.Float;

            case "int":
            case "integer":
            case "number":
                return Type.Integer;

            default:
                throw new Exception("Invalid type");
        }
    }

    public static String getDefaultValue(Type type) {
        switch(type) {
            case Boolean:
                return "false";
            case Float:
                return "0.0";
            case Integer:
                return "1";
        }
        return "TODO";
    }

    public boolean validValue(String value) {
        switch(type) {
            case Boolean:
                return value.matches("true|false");

            case Integer:
                return value.matches("\\d+");

            case Float:
                return value.matches("[-+]?[0-9]+(?:\\.[0-9]+)?");
        }
        return false;
    }
}

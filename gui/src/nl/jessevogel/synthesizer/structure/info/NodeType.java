package nl.jessevogel.synthesizer.structure.info;

public class NodeType {

    public String name;
    public String group;

    public Option[] options;
    public String[] files;

    public static class Option {
        public enum Type { Boolean, Integer, Float }

        public String label;
        public Type type;
        public String description;

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
    }
}
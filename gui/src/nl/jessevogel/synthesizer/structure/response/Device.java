package nl.jessevogel.synthesizer.structure.response;

public class Device {

    public enum Type { MIDI, Input, Output }

    public static final Device NONE = new Device(Type.Input, -1, "<none>", false);

    public Type type;
    public int id;
    public String name;
    public boolean active;

    public Device() {}

    private Device(Type type, int id, String name, boolean active) {
        this.type = type;
        this.id = id;
        this.name = name;
        this.active = active;
    }
}

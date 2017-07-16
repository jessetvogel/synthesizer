package nl.jessevogel.synthesizer.structure.response;

public class Device {

    public enum Type { MIDI, Input, Output }

    public Type type;
    public int id;
    public String name;
    public boolean active;

}

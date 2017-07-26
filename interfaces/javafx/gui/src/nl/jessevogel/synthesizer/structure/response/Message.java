package nl.jessevogel.synthesizer.structure.response;

public class Message {

    public enum Type { Info, Warning, Error }

    public Type type;
    public String message;

}

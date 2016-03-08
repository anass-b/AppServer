package com.medar.coffeecode.app;

public class KeyEvent extends InputEvent {

    private KeyEventType keyEventType;
    private char character;

    public KeyEvent(KeyEventType keyEventType, long windowId, long viewId, long viewTag, char character) {
        super(InputEventType.KEY, windowId, viewId, viewTag);
        this.keyEventType = keyEventType;
        this.character = character;
    }

    public KeyEventType getKeyEventType() {
        return keyEventType;
    }

    public void setKeyEventType(KeyEventType keyEventType) {
        this.keyEventType = keyEventType;
    }

    public char getCharacter() {
        return character;
    }

    public void setCharacter(char character) {
        this.character = character;
    }

    public enum KeyEventType {
        UNDEFINED,
        PRESS,
        RELEASE
    }
}

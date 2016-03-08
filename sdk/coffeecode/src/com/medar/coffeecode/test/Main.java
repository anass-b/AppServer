package com.medar.coffeecode.test;


import com.medar.coffeecode.app.App;

public class Main {

    public static void main(String[] args) {
        App.getInstance().start(new SimpleAppListener());
    }
}

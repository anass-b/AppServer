package com.medar.coffeecode.test;

import com.medar.coffeecode.app.*;
import com.medar.coffeecode.app.Window;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;

// Create am image view by extending the View CoffeeCode class
class ImageView extends View implements View.DrawEventListener {
    private BufferedImage image;

    ImageView(double x, double y, double width, double height, BufferedImage image) {
        super(x, y, width, height);
        this.image = image;
        addDrawEventListener(this);
    }

    @Override
    public void draw(Graphics g) {
        g.drawImage(this.image, 0, 0, null);
    }
}

public class SimpleAppListener implements App.AppListener {
    @Override
    public void appStarted() {
        // Code that runs when the application starts for the first time
        App.getInstance().setQuitAppIfLastWindowDestroyed(true);

        Window window1 = new Window("Simple window 1", 100.0, 100.0, 640.0, 480.0);
        try {
            ImageView imageView1 = new ImageView(0.0, 0.0, window1.getWidth(), window1.getHeight(), ImageIO.read(new File("img/image.png")));
            window1.addView(imageView1);
        }
        catch (Exception e) {
        }
        window1.show();

        Window window2 = new Window("Simple window 2", 250.0, 250.0, 480.0, 640.0);
        try {
            ImageView imageView = new ImageView(0.0, 0.0, window2.getWidth(), window2.getHeight(), ImageIO.read(new File("img/nature.png")));
            // Add draw event listener to the image view...
            imageView.addDrawEventListener(new View.DrawEventListener() {
                @Override
                public void draw(Graphics g) {
                    // Drawing code goes here...
                }
            });
            window2.addView(imageView);
        }
        catch (Exception e) {
        }
        // Adding mouse event listener to the window
        window2.addMouseEventListener(new Window.MouseEventListener() {
            @Override
            public void mouseEventOccurred(MouseEvent e) {
                if (e.getMouseEventType() == MouseEvent.MouseEventType.MOVE) {
                    // Handle move mouse event...
                }
            }
        });
        window2.show();
    }

    @Override
    public void appQuitting() {
        // App quit code goes here..
    }
}

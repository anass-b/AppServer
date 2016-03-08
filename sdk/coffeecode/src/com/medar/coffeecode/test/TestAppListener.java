package com.medar.coffeecode.test;


import com.medar.coffeecode.app.*;
import com.medar.coffeecode.app.Window;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.util.Date;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;

public class TestAppListener implements App.AppListener {

    int posX = 0;
    int posY = 0;
    Timer timer;

    @Override
    public void appStarted() {
        App.getInstance().setQuitAppIfLastWindowDestroyed(true);

        Random rand = new Random();

        String path = "img/";
        File folder = new File(path);
        File[] listOfFiles = folder.listFiles();
        for (File listOfFile : listOfFiles) {
            if (listOfFile.isFile()) {
                String filename = listOfFile.getName();
                try {
                    BufferedImage image = ImageIO.read(new File(path + filename));
                    ImageView view = new ImageView(0.0, 0.0, image.getWidth(), image.getHeight(), image);

                    double x = randNumber(rand, 0, 800);
                    double y = randNumber(rand, 0, 600);
                    Window window = new Window(filename, x, y, image.getWidth(), image.getHeight());
                    window.addKeyEventListener(new Window.KeyEventListener() {
                        @Override
                        public void keyEventOccurred(KeyEvent e) {
                            if (e.getCharacter() == 'q') {
                                App.getInstance().quit();
                            }
                        }
                    });
                    window.addView(view);
                    window.show();
                }
                catch (Exception ignored) {
                }
            }
        }

        /*try {
            Window window = new Window("Nice window", 100.0, 100.0, 640.0, 480.0);

            final BufferedImage image = ImageIO.read(new File("img/image3.png"));
            final View view = new View(0.0, 0.0, window.getWidth(), window.getHeight());
            view.addDrawEventListener(new View.DrawEventListener() {
                @Override
                public void draw(Graphics g) {
                    g.setColor(Color.WHITE);
                    g.fillRect(0, 0, (int)view.getWidth(), (int)view.getHeight());
                    g.drawImage(image, posX, posY, null);
                }
            });

            window.addMouseEventListener(new Window.MouseEventListener() {
                @Override
                public void mouseEventOccurred(MouseEvent e) {
                    if (e.getMouseEventType() == MouseEvent.MouseEventType.MOVE) {
                        posX = (int)(e.getLocation().x - (image.getWidth() / 2));
                        posY = (int)(e.getLocation().y - (image.getHeight() / 2));
                        view.markAsNeedsRedraw();
                    }
                }
            });

            window.addView(view);
            window.show();

            // Uncomment to enable the fast window resizing test
            timer = new Timer();
            timer.scheduleAtFixedRate(new RemindTask(window), new Date(), 15);
        }
        catch (Exception e) {
        }*/
    }

    class RemindTask extends TimerTask {

        Window window;
        double width = 200.0;
        double height = 200.0;

        RemindTask(Window window) {
            this.window = window;
        }

        public void run() {
            width += 50.0;
            height += 50.0;

            if (width >= 800.0 || height >= 800.0) {
                width = 200.0;
                height = 200.0;
            }

            window.resize(width, height);
        }
    }

    public static double randNumber(Random rand, double min, double max) {
        return rand.nextInt(((int)max - (int)min) + 1) + min;
    }

    @Override
    public void appQuitting() {
    }
}

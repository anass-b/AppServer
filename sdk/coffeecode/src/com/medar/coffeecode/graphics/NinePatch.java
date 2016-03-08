package com.medar.coffeecode.graphics;

import com.medar.coffeecode.app.Resources;

import java.awt.*;
import java.awt.image.BufferedImage;

public class NinePatch {

    private static NinePatch instance = new NinePatch();

    public static void render(Graphics2D g2d, String setId, Color fill, double width, double height) {
        BufferedImage topLeft = Resources.get(setId + "_top_left");
        BufferedImage topMiddle = Resources.get(setId + "_top_middle");
        BufferedImage topRight = Resources.get(setId + "_top_right");

        BufferedImage middleLeft = Resources.get(setId + "_middle_left");
        BufferedImage bottomLeft = Resources.get(setId + "_bottom_left");
        BufferedImage bottomMiddle = Resources.get(setId + "_bottom_middle");

        BufferedImage bottomRight = Resources.get(setId + "_bottom_right");
        BufferedImage middleRight = Resources.get(setId + "_middle_right");

        // Title bar
        g2d.drawImage(topLeft, 0, 0, null);
        int titleBarWidth = (int)width - (topLeft.getWidth() + topRight.getWidth());
        for (int i = 0; i < titleBarWidth; i++) {
            g2d.drawImage(topMiddle, i + topLeft.getWidth(), 0, null);
        }
        g2d.drawImage(topRight, (int)width - topLeft.getWidth(), 0, null);

        // Left border
        int leftBorderHeight = (int)height - (topLeft.getHeight() + bottomLeft.getHeight());
        for (int i = 0; i < leftBorderHeight; i++) {
            g2d.drawImage(middleLeft, 0, topLeft.getHeight() + i, null);
        }

        // Bottom left corner
        int bottomLeftCornerY = topLeft.getHeight() + leftBorderHeight;
        g2d.drawImage(bottomLeft, 0, bottomLeftCornerY, null);

        // Bottom border
        int bottomBorderWidth = (int)width - bottomLeft.getWidth() - bottomRight.getWidth();
        for (int i = 0; i < bottomBorderWidth; i++) {
            g2d.drawImage(bottomMiddle, bottomLeft.getWidth() + i, bottomLeftCornerY, null);
        }

        // Bottom right corner
        int bottomRightCornerX = bottomLeft.getWidth() + bottomBorderWidth;
        g2d.drawImage(bottomRight, bottomRightCornerX, bottomLeftCornerY, null);

        // Right border
        for (int i = 0; i < leftBorderHeight; i++ ) {
            g2d.drawImage(middleRight, (int)width - middleRight.getWidth(), topRight.getHeight() + i, null);
        }

        if (fill != null) {
            g2d.fillRect(
                    topLeft.getWidth(),
                    topLeft.getHeight(),
                    (int)width - (topLeft.getWidth() + topRight.getWidth()),
                    (int)height - (topMiddle.getHeight() + bottomMiddle.getHeight()));
        }
    }
}

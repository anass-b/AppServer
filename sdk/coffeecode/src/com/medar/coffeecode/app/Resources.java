package com.medar.coffeecode.app;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;

public class Resources {

    private static Resources instance = new Resources();
    private HashMap<String, BufferedImage> imageHashMap = new HashMap<String, BufferedImage>();
    private final static String resourcesPath = "res/";

    private Resources() {
        File folder = new File(resourcesPath);
        File[] listOfDirs = folder.listFiles();
        for (File root : listOfDirs) {
            if (root.isDirectory()) {
                File[] listOfFiles = root.listFiles();
                for (File file : listOfFiles) {
                    if (file.isFile()) {
                        String filename = file.getName();
                        try {
                            BufferedImage image = ImageIO.read(file);
                            String filenameWithoutExtension = getFilenameWithoutExtension(filename);
                            if (filenameWithoutExtension != null) {
                                imageHashMap.put(root.getName() + "_" + filenameWithoutExtension, image);
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
            else if (root.isFile()) {
                String filename = root.getName();
                try {
                    BufferedImage image = ImageIO.read(root);
                    String filenameWithoutExtension = getFilenameWithoutExtension(filename);
                    if (filenameWithoutExtension != null) {
                        imageHashMap.put(filenameWithoutExtension, image);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static BufferedImage get(String key) {
        return instance.imageHashMap.get(key);
    }

    private String getFilenameWithoutExtension(String filename) {
        if (filename == null || filename.length() == 0) return null;
        int dotIndex = filename.indexOf('.');
        return filename.substring(0, dotIndex);
    }
}

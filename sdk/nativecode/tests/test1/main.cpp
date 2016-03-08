#include <iostream>
#include <nativecode/nativecode.h>

using namespace app;

class ImageView : public View
{
public:
    ImageView(Rect frame, const char *filename) : View(frame)
    {
        _imageSurface = new Surface(filename);
    }
protected:
    virtual void draw()
    {
        getSurface()->blit(_imageSurface, makeRect(0.0, 0.0, _imageSurface->getSize().width, _imageSurface->getSize().height), makePoint(0.0, 0.0));
    }
private:
    Surface *_imageSurface;
};

class TestApplicationDelegate : public ApplicationDelegate, MouseEventDelegate, KeyEventDelegate
{
public:
    virtual void applicationStarted()
    {
        _mainWindow = new Window(makeRect(50.0, 50.0, 480.0, 640.0), kWindowVisibilityVisible);
        ImageView *imageView2 = new ImageView(makeRect(70.0, 70.0, 350.0, 382.0), "img/image.png");
        _mainWindow->addView(imageView2);
        
        _secondWindow = new Window(makeRect(200.0, 200.0, 640.0, 480.0), kWindowVisibilityVisible);
        ImageView *imageView1 = new ImageView(makeRect(70.0, 70.0, 350.0, 382.0), "img/space.png");
        _secondWindow->addView(imageView1);
    }
    
    virtual void mouseEventOccured(MouseEvent *e)
    {
        if (e == nullptr) return;
        if (e->type == kMouseEventTypePress) {
            // Handle mouse press event...
        }
        else if (e->type == kMouseEventTypeMove) {
            // Handle mouse move event...
        }
    }
    
    virtual void keyEventOccured(KeyEvent *e)
    {
        // Handle key event...
    }
private:
    ImageView* _imageView1 = nullptr;
    ImageView* _imageView2 = nullptr;
    Window* _mainWindow;
    Window* _secondWindow;
};

int main(int argc, const char * argv[])
{
    return Application::getSingleton()->run(new TestApplicationDelegate());
}

#include "olcPixelGameEngine.h"
#include <math.h>
#include <omp.h>

class Mandelbrot : public olc::PixelGameEngine
{
public:
    Mandelbrot()
    {
        sAppName = "mandelbrot";
    }
private:
    int MAXITERATIONS = 10000;
    double SAT = 0.9;
    int COLORS = 50;
    double rangeX = 4;
    double rangeY;
    long double xmin; // x goes from xmin to xmax
    long double ymin;
    long double xmax;
    long double ymax;
    bool go = false;
    double offsetX = 0.0;
	double offsetY = 0.0;
    double scaleX = 1.0;
    double scaleY = 1.0;

    bool OnUserCreate() override
    {
        xmin = -rangeX/2; // Start at negative half the width and height
        rangeY = (rangeX * ScreenHeight()) / ScreenWidth();
        ymin = -rangeY/2;
        xmax = -xmin; + rangeX;
        ymax = -ymin; + rangeY;
        offsetX = 0;
		offsetY = 0;
        return true;
    }

    void WorldToScreen(double fWorldX, double fWorldY, int &nScreenX, int &nScreenY)
	{
		nScreenX = (int)((fWorldX - offsetX) * scaleX);
		nScreenY = (int)((fWorldY - offsetY) * scaleY);
	}

	// Convert coordinates from Screen Space --> World Space
	void ScreenToWorld(int nScreenX, int nScreenY, double &fWorldX, double &fWorldY)
	{
		fWorldX = ((double)nScreenX / scaleX) + offsetX;
		fWorldY = ((double)nScreenY / scaleY) + offsetY;
	}

    bool OnUserUpdate(float fElapsedTime) override
    {
        int width = ScreenWidth();
        int height = ScreenHeight();

        if(GetKey(olc::Key::Q).bHeld)
        {
            scaleX *= 1.1;
            scaleY *= 1.1;
            go = true;
        }

        if(go)
        {
            long double newOriginX =  -1.74999841099374081749002483162428393452822172335808534616943930976364725846655540417646727085571962736578151132907961927190726789896685696750162524460775546580822744596887978637416593715319388030232414667046419863755743802804780843375;
            long double newOriginY = -0.00000000000000165712469295418692325810961981279189026504290127375760405334498110850956047368308707050735960323397389547038231194872482690340369921750514146922400928554011996123112902000856666847088788158433995358406779259404221904755;

            int ZOOMFACTOR = 2;
            double zoomDiffX = (xmax - xmin) / (2 * ZOOMFACTOR); // We use these to determine new xy range
            double zoomDiffY = (ymax - ymin) / (2 * ZOOMFACTOR);

            xmin = newOriginX - zoomDiffX;
            ymin = newOriginY - zoomDiffY;
            xmax = newOriginX + zoomDiffX;
            ymax = newOriginY + zoomDiffY;

            go = false;

        }

        double dx = (xmax - xmin) / (width);
        double dy = (ymax - ymin) / (height);

        // Start y
        double y = ymin;
        #pragma omp parallel for collapse(3)
        for (int j = 0; j < height; j++)
        {
            // Start x
            double x = xmin;
            for (int i = 0; i < width; i++)
            {
                // Now we test, as we iterate z = z^2 + cm does z tend towards infinity?
                double a = x;
                double b = y;
                int n = 0;
                while (n < MAXITERATIONS)
                {
                    double aa = a * a;
                    double bb = b * b;
                    if (aa + bb > 4)// All numbers outside circle radius 2 tend to infinity
                        break;  // Bail
                    double twoab = 2.0 * a * b;
                    a = aa - bb + x;
                    b = twoab + y;
                    n++;
                }
                // We color each pixel based on how long it takes to get to infinity
                // If we never got there, let's pick the color black
                int pixel_sx,pixel_sy;
                //WorldToScreen(i, j, pixel_sx, pixel_sy);
                if (n == MAXITERATIONS) {
                    Draw(i,j,olc::BLACK);
                    
                } else {
                    int c = n % COLORS;
                    Draw(i,j,olc::Pixel(c, COLORS * SAT, COLORS));
                }
                x += dx;
            }
            y += dy;
        }
        return true;
    }
};

int main()
{
    Mandelbrot p;
    int pix = 4;
    if(p.Construct(360/pix,360/pix,pix,pix))
        p.Start();
    return 0;
}
#include "..\PhaseCongruency\phase.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>



using namespace cv;
using namespace std;


static void help()
{
    cout << "\nThis program seek for letter bounding box on image\n"
        << "Call:\n"
        << "/.edge input_image_name [output_image_name]"
        << endl;
}

int main(int argc, char** argv)
{
    try
    {
        const String inFileKey = "@inputImage";
        const String outFileKey = "@outputImage";
        const String keys =
            "{help h usage ?    |      | print this message }"
            "{" + inFileKey + " |<none>| input image        }"
            "{" + outFileKey + "|<none>| output image       }";
        CommandLineParser parser(argc, argv, keys);
        if (parser.has("help") || !parser.has(inFileKey))
        {
            help();
            return 0;
        }
        const string inputFileName = parser.get<String>(inFileKey);

        Mat image = imread(inputFileName, IMREAD_GRAYSCALE);
        if (image.empty())
        {
            cout << "Cannot read image file " << inputFileName << endl;
            help();
            return -1;
        }
       
        auto size = image.size() * 3;
        resize(image, image, size);
        
        PhaseCongruency pc(image.size(), 4, 6);

        Mat edges, corners;
        pc.feature(image, edges, corners);

        if (parser.has(outFileKey))
        {
            const string outputFileName = parser.get<String>(outFileKey);
            imwrite(outputFileName, edges);
        }
        else
        {
            namedWindow("image");
            imshow("image", edges);
        }
    }
    catch (Exception& e)
    {
        const char* err_msg = e.what();
        std::cout << "Exception caught: " << err_msg << std::endl;
    }
    waitKey();

    return 0;
}


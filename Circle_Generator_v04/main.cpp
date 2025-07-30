#include <circle_generator_v04.h>


/**
 * path has to be manually modified in circle_geneartor.cpp/loop_circle_generator(...)
 */

/**
 * @author Tai Zhang
 * @version .04
 * version update compared to v00:
 *      - all files are merged into single file, seperated by endl;
 *      - multiple number of circles of different radii on same sample
 *      - percentage based generation of pixels in circle
 *      - changed labeling, including radii + x_coord + y_coord of each circle
 */

int main()
{
    Circle_generator test;

    // saving output location
    test.output_directory = "..\\Circle_Generator_v04\\generated_test_data\\";
    test.output_name = "test_circles";
    test.output_type = "txt";

    // set to true to print content in terminal
    test.visualization_status = false;

    /**
     * @brief main function to call to generate circles
     * @param a = number of positive samples
     * @param b = number of negative samples
     * @param size of canvas n*n
     * @param minimum percentage of pixels
     * @param maximum percentage of pixels
     * @param noise rate
     * @param minimum number of circles of each radii on screen
     * @param maximum number of circles of each radii on screen
     * @param radii of circle
     * @param number of cicle with different radii
     */
//    const int radii_size = 2;
//    int radii[radii_size] = {15, 25};
//    test.loop_circle_generator(40, 10, 300, 70, 90, 0.005, 0, 20, radii, radii_size, "test_circles", "txt");

    // small sample for testing test_data in CNN
    const int radii_size = 2;
    int radii[radii_size] = {15, 25};
    test.loop_circle_generator(6, 2, 300, 70, 90, 0.005, 0, 20, radii, radii_size);

//    const int radii_size = 2;
//    int radii[radii_size] = {4, 6};
//    test.loop_circle_generator(35, 15, 50, 70, 90, 0.01, 0, 5, radii, radii_size, "test_circles", "txt");

    // circle generation for analysis of downstream HCT (Hough Circle Transform)
    // const int radii_size = 2;
    // int radii[radii_size] = {15, 25};
//    test.loop_circle_generator(35, 0, 100, 80, 100, 0.005, 0, 5, radii, radii_size, "HCT_sample_80_100", "txt");
//    test.loop_circle_generator(35, 0, 100, 60, 80, 0.005, 0, 5, radii, radii_size, "HCT_sample_60_80", "txt");
//    test.loop_circle_generator(35, 0, 100, 40, 60, 0.005, 0, 5, radii, radii_size, "HCT_sample_40_60", "txt");
//    test.loop_circle_generator(35, 0, 100, 20, 40, 0.005, 0, 5, radii, radii_size, "HCT_sample_20_40", "txt");
    // test.loop_circle_generator(35, 0, 100, 0, 20, 0.005, 0, 5, radii, radii_size, "HCT_sample_00_20", "txt");

//    // single circle in small quadrant for no segmentation
//    const int radii_size = 1;
//    int radii[radii_size] = {8};
//    test.loop_circle_generator(5, 0, 30, 70, 90, 0.005, 1, 3, radii, radii_size, "test_circles", "txt");

    return 0;
}


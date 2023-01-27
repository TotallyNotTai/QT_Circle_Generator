<<<<<<< HEAD
#include <circle_generator_v04.h>

#include <iostream>

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

    /**
     * @brief main function to call to generate circles
     * @param a = number of positive samples
     * @param b = number of negative samples
     * @param size of canvas n*n
     * @param minimum percentage of pixels
     * @param maximum percentage of pixels
     * @param noise rate
     * @param maximum number of circles of each radii on screen
     * @param radii of circle
     * @param data type
     */
    const int radii_size = 2;
    int radii[radii_size] = {4, 6};
    test.loop_circle_generator(8, 2, 50, 60, 80, 0.01, 4, radii, radii_size, "txt");


    return 0;
}

=======
#include <circle_generator_v04.h>

#include <iostream>

/**
 * path has to be manually modified in circle_geneartor.cpp/loop_circle_generator(...)
 */

/**
 * @author Tai Zhang
 * @version .02
 * version update:
 *      - all files are merged into single file, seperated by endl;
 *      - multiple number of circles of different radii on same sample
 *      - percentage based generation of pixels in circle
 */

int main()
{
    Circle_generator test;

    /**
     * @brief main function to call to generate circles
     * @param a = number of positive samples
     * @param b = number of negative samples
     * @param size of canvas
     * @param minimum number of pixels
     * @param maximum number of pixels
     * @param noise rate
     * @param maximum number of circles each radii on screen
     * @param radii of circle
     * @param data type
     */
    const int radii_size = 2;
    int radii[radii_size] = {4, 6};
    test.loop_circle_generator(8, 2, 50, 60, 80, 0.01, 4, radii, radii_size, "txt");


    return 0;
}

>>>>>>> 8a8adb4892b4e296d22f3411070b6c0fcfd0d77a

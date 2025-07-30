#include <circle_generator_v01.h>

#include <iostream>

/**
 * path has to be manually modified in circle_geneartor.cpp/loop_circle_generator(...)
 */

/**
 * version update:
 *      - all files are merged into single file, seperated by endl;
 */

int main()
{
    Circle_generator test;

    /**
     * @brief main function to call to generate circles
     * @param a = number of loops
     * @param size of canvas
     * @param minimum number of pixels
     * @param maximum number of pixels
     * @param noise rate
     * @param radius of circle
     * @param data type
     */
    test.loop_circle_generator(200, 30, 10, 30, 0.01, 5, "txt");


//    /**
//     * @brief function to generate empty canvas with noise
//     * @param inum = number of loops
//     * @param size of canvas
//     * @param noise rate
//     * @param data type
//     */
//    test.loop_negative_samples(5000, 30, 0.01, "txt");

    return 0;
}


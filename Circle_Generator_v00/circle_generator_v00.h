#ifndef CIRCLE_GENERATOR_V00_H
#define CIRCLE_GENERATOR_V00_H

#include <iostream>
#include <math.h>
#include <random>
#include <vector>
#include <string>
#include <fstream>

/**
* @author Tai Zhang
* @version 1.0
*/


/**
 * @brief pseudo random number generator depending on interval input based on mersenne twister
 * @param lower and upper bounds of the interval
 * @return random number between the two given numbers
*/
class mt_rand_generator_class
{
public:
    static double mt_rand_generator(int a1, int a2);
};



class Circle_generator{
    public:
        /**
         * @brief saving format of coordinates
        */
        struct Point_Coordinates{
            int x, y;

            Point_Coordinates(int paramx, int paramy) : x(paramx),y(paramy){};
        };

        //quadratic canvas
        struct Canvas{
            int**   canvas_array;
            int     canvas_size;
            std::vector<Point_Coordinates> final_list_of_coord;
        };

        std::vector<Point_Coordinates> list_of_coord;

        int min_num_points, max_num_points;

        Canvas final_canvas;


        /**
         * @brief draw the canvas
        */
        void canvas_drawer();


        /**
         * @brief generates background noise for canvas
         * @param the probability of a noise appearing in a pixel of canvas is P(a), a is a float number in interval [0, 1], recommed to around 0.02
         * saves values in final_canvas.final_list_of_coord
         */
        void noise_generator(float a);


        /**
         * @brief helping function for main drawing circle function, calculation of additional pixel in circle based on bresenham circle algorithm
         * @param x
         * @param y
         * @param p
         * @param q
         */
        void bresenham_help(int x, int y, int p, int q);

        /**
         * @brief circle_generator based on bresenham circle algorithm
         * @param minimum number of pixels
         * @param maximum number of pixels
         * @param radius of circle
         * @param x coord of circle center
         * @param y coord of circle center
         * saves values in final_canvas.final_list_of_coord
         */
        void circle_generator(int imin_dots, int imax_dots, int ir, int ip, int iq);

        /**
         * @brief generates external txt file from final_canvas.canvas_array
         * @param number for different names while looping
         * @param output type (either .txt or .png
         * reads coordinates from final_canvas.final_list_of_coord and sets values to 1
         */
        void external_file_generator(int a, std::string data_path, std::string data_name, std::string output_type="txt");

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
        void loop_circle_generator(int a, int isize, int imin_num_dots, int imax_num_dots, float inoise, int iradius, std::string output_type);

        /**
         * @brief function to generate empty canvas with noise
         * @param inum = number of loops
         * @param size of canvas
         * @param noise rate
         * @param data type
         */
        void loop_negative_samples(int inum, int isize, float inoise, std::string output_type);

        /**
         * @brief empties folder with previous files
         * @param path to folder
         */
        void file_remover(std::string path);
};

#endif // CIRCLE_GENERATOR_V00_H

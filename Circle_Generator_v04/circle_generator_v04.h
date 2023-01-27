#ifndef CIRCLE_GENERATOR_V04_H
#define CIRCLE_GENERATOR_V04_H

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
         *
         * main looping function, number of loops, canvas size, min and max of pixels in circle, noise rate, radius
         * ideally to use maxium space iradius = isize/4
         * recommend to set noise at 0.01-0.05
         * generates single file with each line a generated test data
         * generates both positive and negative samples
         *
         * file format: n, m_1, m_2, ..., m_n, data
         * n: number of lables, in our specific case 2
         * m_1, m_2, ..., m_n: individual lable values
         * data: rest of data in single line seperated by commas, for example a 30x30 field generates a single string of 900 zeroes/ones seperated by 899 commas
         */
        void loop_circle_generator(int iloops, int inegloops, int isize, int imin_num_dots, int imax_num_dots, float inoise, int max_num_circle,
                                   int* iradii, int iradii_size, std::string output_type);

        /**
         * @brief empties folder with previous files
         * @param path to folder
         */
        void file_remover(std::string path);
};

#endif // CIRCLE_GENERATOR_V04_H

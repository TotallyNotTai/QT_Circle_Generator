#ifndef CIRCLE_GENERATOR_V04_H
#define CIRCLE_GENERATOR_V04_H

#include <cmath>
#include <random>
#include <vector>
#include <string>

/**
* @author Tai Zhang
* @version 4.1
*/


/**
 * @brief The Circle_generator class
 */
class Circle_generator{
    public:
        /**
         * @brief main function to call to generate circles
         * @param iloops = number of positive samples
         * @param inegloops = number of negative samples
         * @param isize = size of canvas
         * @param imin_num_dots = minimum number of pixels
         * @param imax_num_dots = maximum number of pixels
         * @param inoise = noise rate
         * @param max_num_circle = maximum number of circles of each radii on screen
         * @param iradii = list of radii of circles
         * @param iradii_size = len of iradii
         *
         * main looping function, number of loops, canvas size, min and max of pixels in circle, noise rate, radius
         * ideally to use maxium space iradius = isize/4
         * recommend to set noise at 0.01-0.05
         * generates single file with each line a generated test data
         * generates both positive and negative samples
         *
         * file format: r_1, x_1, y_1,..., data
         * r_n = radius of circle n
         * x_n, y_n = coord of circle center of circle n
         * file format: n, m_1, m_2, ..., m_n, data
         * n: number of lables, in our specific case 2
         * m_1, m_2, ..., m_n: individual lable values
         * data: rest of data in single line seperated by commas, for example a 30x30 field generates a single string of 900 zeroes/ones seperated by 899 commas
         */
        void loop_circle_generator(int iloops, int inegloops, int isize, int imin_num_dots, int imax_num_dots, float inoise, int min_num_circles,
                                   int max_num_circle, int* iradii, int iradii_size);

        std::string output_directory;
        std::string output_name;
        std::string output_type;

        bool visualization_status;

    private:
        /**
         * @brief saving format of coordinates
        */
        struct Point_Coordinates{
            int x, y;

            Point_Coordinates(int paramx, int paramy) : x(paramx),y(paramy){};
        };

        // quadratic canvas
        struct Canvas{
            std::vector<std::vector<int>>   canvas_array;
            int canvas_size;
            std::vector<Point_Coordinates>  final_list_of_coord;
        };

        std::vector<Point_Coordinates> list_of_coord;

        int min_num_points, max_num_points;

        int** total_rad_coord_array;

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
         * @brief empties folder with previous files
         * @param path to folder
         */
        void file_remover(std::string path);

        /**
         * @brief for mt_rand_generator
         */
        std::mt19937 m_rng;

        double mt_rand_generator(double a1, double a2);
};

#endif // CIRCLE_GENERATOR_V04_H

#ifndef CIRCLE_GENERATOR_V05_H
#define CIRCLE_GENERATOR_V05_H

#include <cmath>
#include <random>
#include <vector>
#include <string>

/**
* @author Tai Zhang
* @version 5.0
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
         * changes for v05:
         * instead of singular file with all data including image of empty zeroes,
         * generating 2 files, one with all the coordinates of circle center, radii for all images, number of pixels
         * second one with all the coordinates of each actual displayed circle pixels + noise coordinates
         *
         * first file format: n, m_i, r_i, x_i, y_i, ...
         * n = number of circles in image, set to -1 if no circles
         * m = number of displayed pixels in circle i
         * r_i = radius of circle i
         * x_i, y_i = coord of circle center of circle n
         *
         * second file format: xn_m, yn_m, ...
         * x, y : coordinate of pixel
         * n : n_th circle of the image
         * if n = 0: coordinates of noises
         * if n > 0: coordinates of n_th circle of image
         * m : m_th pixel of the group
         *
         * first file name has ending "_cc" (circle centers), second file name has ending "_p" (pixels)
         *
         * another change is to discard the struct Canvas existing in previous version, replaced by final_list_coord_p
         * canvas_drawer is modified to generate an image if called
         */
        void loop_circle_generator(int iloops, int inegloops, int isize, int imin_num_dots, int imax_num_dots, float inoise, int min_num_circles,
                                   int max_num_circle, int* iradii, int iradii_size);


        std::string output_directory;
        std::string output_name;
        std::string output_type;

        bool visualization_status;

    private:
        //all information for output
        struct Output_cc_p{
            // std::vector<Point_Coordinates> final_coords_cc;
            // int r_cc;
            // int n_cc;

            int image_size;

            std::vector<int> final_coords_cc;

            std::vector<std::vector<int>> final_coords_p;
            std::vector<std::vector<int>> noise_coords_p;
        };

        std::vector<std::vector<int>> temp_bres_vec;

        int n_cc;

        int min_num_points, max_num_points;

        Output_cc_p output_struct;

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
        void circle_generator_v05(int imin_dots, int imax_dots, int ir, int ip, int iq);

        /**
         * @brief empties folder with previous files
         * @param path to folder
         */
        void file_remover(std::string path);

        void printCoordinateMap(const std::vector<std::vector<int>>& coordinates, int imageSize);

        /**
         * @brief for mt_rand_generator
         */
        std::mt19937 m_rng;

        double mt_rand_generator(double a1, double a2);
};

#endif // CIRCLE_GENERATOR_V05_H

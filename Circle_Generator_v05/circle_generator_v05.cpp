#include "circle_generator_v05.h"

#include <iostream>
#include <math.h>
#include <random>
#include <vector>
#include <string>
#include <fstream>
#include <QImage>
#include <filesystem>

/**
 * generates test data in form of circles with [n, m] number of pixels with noise
 * intended for generating data for RICH
 * @author Tai Zhang
 * @version 1.0
*/
double Circle_generator::mt_rand_generator(double a1, double a2){
    std::uniform_real_distribution<double> dis(a1, a2);
    double rand_number = dis(m_rng);
    return rand_number;
};


void Circle_generator::noise_generator(float a){
    // _v05 changes
    try{
        if (0 < a and a < 1){
            // adding a randomness to number of noise pixels
            // for now an uniform spread of a 10% total interval around the given value a
            float r = 0.05;
            int base_noise_r = output_struct.image_size * output_struct.image_size * a;
            int noise_r = int(mt_rand_generator(int(base_noise_r * (1 - r)), int(base_noise_r * (1 + r))));

            for(int j=0; j<noise_r; j++){
                int x_noise = mt_rand_generator(0, output_struct.image_size);
                int y_noise = mt_rand_generator(0, output_struct.image_size);

                output_struct.noise_coords_p.push_back({x_noise, y_noise});
            }
        } else {
            std::cerr << "inoise value has to be between 0 and 1. Please enter a valid value." << std::endl;
        }
    } catch (std::exception&e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}

// writes circle with bresenham algorithm
void Circle_generator::bresenham_help(int x, int y, int p, int q){
    // _v05 changes
    temp_bres_vec.push_back({x+p, y+q});
    temp_bres_vec.push_back({-x+p, y+q});
    temp_bres_vec.push_back({x+p, -y+q});
    temp_bres_vec.push_back({-x+p, -y+q});
    temp_bres_vec.push_back({y+p, x+q});
    temp_bres_vec.push_back({-y+p, x+q});
    temp_bres_vec.push_back({y+p, -x+q});
    temp_bres_vec.push_back({-y+p, -x+q});
}


void Circle_generator::circle_generator(int imin_dots, int imax_dots, int ir, int ip, int iq){
    int x, y, d;

    d = 3-2*ir;
    x = 0;
    y = ir;

    bresenham_help(x, y, ip, iq);

    while (x<y){
        if(d<=0){
            d=d+(4*x)+6;
            bresenham_help(++x, y, ip, iq);
        }
        else{
            d=d+(4*(x-y))+10;
            bresenham_help(++x, --y, ip, iq);
        }
    }

    //sort elements, then remove duplicate elements

    //generate a new list of random points between given [a, b]
    int rand_value = static_cast<int>(mt_rand_generator(imin_dots, imax_dots));

    // _v05 change
    int rand_num_pixels = temp_bres_vec.size()*rand_value/100;

    // shuffle and take the first rand_num_pixels elements from the list
    std::shuffle(temp_bres_vec.begin(), temp_bres_vec.end(), m_rng);

    output_struct.final_coords_p.insert(output_struct.final_coords_p.end(), temp_bres_vec.begin(), temp_bres_vec.begin() + rand_num_pixels);

    n_cc = rand_num_pixels;

    temp_bres_vec.clear();
}

/**
 * @brief Circle_generator::printCoordinateMap
 * @param coordinates
 * @param imageSize
 *
 * print a canvas of hits from a given list of coordinates
 */
void Circle_generator::printCoordinateMap(const std::vector<std::vector<int>>& coordinates, int imageSize){
    std::vector<std::vector<char>> canvas(imageSize, std::vector<char>(imageSize, '.'));

    for (const auto& coord : coordinates){
        if (coord.size() == 2) {
            int x = coord[0];
            int y = coord[1];
            if (x >= 0 && x < imageSize && y >= 0 && y < imageSize){
                canvas[y][x] = 'X'; // Note: y is row, x is column
            }
        }
    }

    for (int y = 0; y < imageSize; ++y){
        for (int x = 0; x < imageSize; ++x){
            std::cout << canvas[y][x];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void Circle_generator::file_remover(std::string dir_path){
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
        std::filesystem::remove_all(entry.path());
}

/**
 * main looping function, number of loops, canvas size, min and max of pixels in circle, noise rate, radius
 * generate both positvee and negative samples
 * iloops = number of positive samples
 * inegloops = number of negative samples
 * isize = size of each sample, nxn
 * a percentage of pixels for each circle are actually drawn on the canvas randomly
 * imin_num_dots = lower bound of circle percentage
 * imax_num_dots = upper bound of circle percentage
 * inoise = random background noise for each sample
 * min_num_circles = minimum amount of circles of each radii
 * max_num_circles = maximum amount of circles of each radii
 * iradii = list of all radii
 * iradii_size = size of iradii
 *
 * ideally to use maxium space iradius = isize/4
 * recommend to set noise at 0.01-0.05
 * generates single file with each line a generated test data
 * generates both positive and negative samples
 *
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
 */
void Circle_generator::loop_circle_generator(int iloops, int inegloops, int isize, int imin_num_dots, int imax_num_dots, float inoise, int min_num_circles,
                                             int max_num_circles, int* iradii, int iradii_size){

    //define size of image
    output_struct.image_size = isize;

    // set mt_rnd_genertor
    std::random_device dev;
    m_rng.seed(dev());

    // output handling
    std::string data_path = output_directory;
    std::string data_name = output_name;

    // remove old content from output folder
    file_remover(data_path);

    std::ofstream fw_cc(data_path + data_name + "_cc" + "." + output_type, std::ofstream::out);
    std::ofstream fw_p(data_path + data_name + "_p" + "." + output_type, std::ofstream::out);

    // generates positive samples ##############################################################################################
    for (int i=0; i<iloops; i++){
        noise_generator(inoise);

        int* num_circle_array = (int*)calloc(iradii_size, sizeof(int));

        // generate a number between 0 and max_num_circles for the number of circles appearing on screen
        for (int j=0; j<iradii_size; j++){
            num_circle_array[j] = mt_rand_generator(min_num_circles, max_num_circles+1);
        }

        // list for future circle coord and radii size
        // r, x_coord, y_coord
        int total_circle_num = 0;
        for (int k=0; k<iradii_size; k++){
            total_circle_num += num_circle_array[k];
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // added with v05: vector (r, x, y, n) for file_cc
        std::vector <int> circle_vector_cc;

        for (int r=0; r<iradii_size; r++){
            // draw the full circle inside the canvas
            int min_coord = iradii[r];
            int max_coord = isize-iradii[r];

            // draw circle
            for (int k=0; k<num_circle_array[r]; k++){
                //variance in circle location
                int x_coord = mt_rand_generator(min_coord, max_coord);
                int y_coord = mt_rand_generator(min_coord, max_coord);

                circle_generator(imin_num_dots, imax_num_dots, iradii[r], x_coord, y_coord);

                // added with v05: vector (r, x, y, n) for file_cc
                std::vector<int> temp_vec_cc = {iradii[r], x_coord, y_coord, n_cc};

                output_struct.final_coords_cc.insert(output_struct.final_coords_cc.end(), temp_vec_cc.begin(), temp_vec_cc.end());
            }
        }

        // ignore pixels outside of image range
        // backwards iteration to solve accidental skipping over elements due to erasing
        for (int k = static_cast<int>(output_struct.final_coords_p.size()) - 1; k >= 0; --k) {
            if (output_struct.final_coords_p[k][0] < 0 || output_struct.image_size <= output_struct.final_coords_p[k][0] ||
                output_struct.final_coords_p[k][1] < 0 || output_struct.image_size <= output_struct.final_coords_p[k][1]) {
                output_struct.final_coords_p.erase(output_struct.final_coords_p.begin() + k);
            }
        }

        if (fw_p.is_open())
        {
            // store array contents to file
            std::string final_string;

            //_v05 changes
            std::ostringstream stream;

            // coords -> final_string
            for(auto i : output_struct.final_coords_p){
                std::copy(i.begin(), i.end(), std::ostream_iterator<int>(stream, ","));
            }

            // noise -> final_string
            for(auto i : output_struct.noise_coords_p){
                std::copy(i.begin(), i.end(), std::ostream_iterator<int>(stream, ","));
            }

            final_string = stream.str();
            //remove final ","
            final_string.erase(final_string.length() - 1);

            output_struct.noise_coords_p.clear();

            final_string += "\n";
            fw_p << final_string;
            final_string = "";
        }
        else {
            std::cout << "Problem with opening file_p" << std::endl;
        }


        // _v05 changes
        if (fw_cc.is_open()){
            std::string final_string;

            if(total_circle_num != 0){
                final_string += std::to_string(total_circle_num) + ",";

                std::ostringstream stream;
                std::copy(output_struct.final_coords_cc.begin(), output_struct.final_coords_cc.end(),
                          std::ostream_iterator<int>(stream, ","));
                final_string = stream.str();
                // remove final ","
                final_string.erase(final_string.length() - 1);
            } else {
                final_string += "-1,0,0,0";
            }
            final_string += "\n";
            fw_cc << final_string;
            final_string = "";
        }
        else {
            std::cout << "Problem with opening file_cc" << std::endl;
        }

        output_struct.final_coords_p.clear();
        output_struct.final_coords_cc.clear();

        if (i%100==0){
            std::cout << "Writing positive sample file Nr. " << i << std::endl;
        }

        free(num_circle_array);
    }

    // generate negative samples #####################################################################
    std::cout << "Generating negative samples" << std::endl;
    for (int i=0; i<inegloops; i++){
        noise_generator(inoise);

        if (fw_cc.is_open())
        {
            // changed in v05 to only return "-1,0,0,0" for each negative sample
            std::string final_string = "-1,0,0,0\n";

            fw_cc << final_string;
        }
        else std::cout << "Problem with opening file" << std::endl;

        if (fw_p.is_open()){
            std::string final_string;

            //_v05 changes
            std::ostringstream stream;

            // noise -> final_string
            for(auto i : output_struct.noise_coords_p){
                std::copy(i.begin(), i.end(), std::ostream_iterator<int>(stream, ","));
            }

            final_string = stream.str();
            //remove final ","
            final_string.erase(final_string.length() - 1);

            output_struct.noise_coords_p.clear();

            final_string += "\n";
            fw_p << final_string;
            final_string = "";
        }
        else {
            std::cout << "Problem with opening file_p" << std::endl;
        }

        output_struct.final_coords_p.clear();

        if (i%100==0){
            std::cout << "Writing negative sample file Nr. " << i << std::endl;
        }
    }

    fw_cc.close();
    fw_p.close();

    // Visualization of the generated sample
    if (visualization_status) {
        std::string p_file_path = data_path + data_name + "_p" + "." + output_type;
        std::ifstream fr_p(p_file_path);

        if (fr_p.is_open()) {
            for (int i = 0; i < iloops + inegloops; ++i) {
                QImage image(output_struct.image_size, output_struct.image_size, QImage::Format_RGB32);
                image.fill(Qt::white);

                std::string p_line;

                fr_p.seekg(std::ios::beg);
                for (int skip = 0; skip < i; ++skip) std::getline(fr_p, p_line);

                if (std::getline(fr_p, p_line)) {
                    std::stringstream ss_p(p_line);
                    std::string segment_p;
                    int current_coord = 0;
                    int x = -1, y = -1;

                    while (std::getline(ss_p, segment_p, ',')) {
                        if (!segment_p.empty()) {
                            int coord_val = std::stoi(segment_p);
                            if (current_coord % 2 == 0) { // x-coordinate
                                x = coord_val;
                            } else { // y-coordinate
                                y = coord_val;
                                if (x >= 0 && x < output_struct.image_size && y >= 0 && y < output_struct.image_size) {
                                    image.setPixel(x, y, Qt::blue);
                                } else {
                                    std::cerr << "Warning: Pixel coordinate out of bounds: (" << x << ", " << y << ") for positive sample " << i << std::endl;
                                }
                                x = -1; // Reset x for the next pair
                            }
                            current_coord++;
                        }
                    }

                    std::string image_file_path = data_path + data_name + "_" + std::to_string(i) + ".png";
                    image.save(QString::fromStdString(image_file_path));
                    std::cout << "Saved visualization to: " << image_file_path << std::endl;
                } else {
                    std::cerr << "Error reading line " << i << " from _p file." << std::endl;
                }
            }
            fr_p.close();
        } else {
            std::cerr << "Error opening pixel data file for visualization." << std::endl;
        }
    }
}

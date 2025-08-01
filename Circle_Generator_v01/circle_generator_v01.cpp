#include "circle_generator_v01.h"

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
double mt_rand_generator_class::mt_rand_generator(int a1, int a2){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<double> dis(a1, a2);

    double rand_number = dis(rng);

    return rand_number;
};

void Circle_generator::canvas_drawer(){
    int a = final_canvas.canvas_size;
    final_canvas.canvas_array = (int**)calloc(a, sizeof(int*));
    for (int i=0;i<a;i++){
        final_canvas.canvas_array[i] = (int*)calloc(a, sizeof(int));
    }
}

void Circle_generator::noise_generator(float a){
    try {
        if (0<a and a<1){
            for(int i=0;i<final_canvas.canvas_size;i++){
                for(int j=0;j<final_canvas.canvas_size;j++){
                    if (mt_rand_generator_class::mt_rand_generator(0,1)>1-a){
                        final_canvas.final_list_of_coord.push_back(Point_Coordinates(i,j));
                    }
                }
            }
        }
        else{
            std::cerr << "Input value outside of interval [0, 1] in Circle_drawer/main.cpp, function noise_generator";
        }
    } catch (std::exception&e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}

//writes circle with bresenham algorithm
void Circle_generator::bresenham_help(int x, int y, int p, int q){
    list_of_coord.push_back(Point_Coordinates(x+p, y+q));
    list_of_coord.push_back(Point_Coordinates(-x+p, y+q));
    list_of_coord.push_back(Point_Coordinates(x+p, -y+q));
    list_of_coord.push_back(Point_Coordinates(-x+p, -y+q));
    list_of_coord.push_back(Point_Coordinates(y+p, x+q));
    list_of_coord.push_back(Point_Coordinates(y+p, -x+q));
    list_of_coord.push_back(Point_Coordinates(-y+p, x+q));
    list_of_coord.push_back(Point_Coordinates(-y+p, -x+q));
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
    int rand_value = static_cast<int>(mt_rand_generator_class::mt_rand_generator(imin_dots, imax_dots));

    //catch if rand_value > list_of_coord.size
    if (rand_value>static_cast<int>(list_of_coord.size())){
        rand_value = list_of_coord.size();
    }

    for(int i=0;i<rand_value;i++){
        final_canvas.final_list_of_coord.push_back(list_of_coord[mt_rand_generator_class::mt_rand_generator(0, list_of_coord.size()-1)]);
    }


//    //checking content of each final_canvas.final_list_of_coord

//    std::cout << "final_canvas.final_list_of_coord ";

//    for (int i=0;i<final_canvas.final_list_of_coord.size();i++){
//        std::cout << final_canvas.final_list_of_coord[i].x << " " << final_canvas.final_list_of_coord[i].y << " ";
//    }

//    std::cout << std::endl << std::endl;


}

void Circle_generator::file_remover(std::string dir_path){
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
        std::filesystem::remove_all(entry.path());
}

/**
 * main looping function, number of loops, canvas size, min and max of pixels in circle, noise rate, radius
 * generate both positvee and negative samples
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
void Circle_generator::loop_circle_generator(int iloops, int isize, int imin_num_dots, int imax_num_dots, float inoise, int iradius, std::string output_type){
    //define size of canvas
    final_canvas.canvas_size = isize;
    min_num_points = imin_num_dots;
    max_num_points = imax_num_dots;

    //to be fixed individually for each user
//    std::string data_path = "C:\\Users\\Tai\\Documents\\QT\\Circle_Generator\\Circle_Generator_v01\\generated_test_data\\";
    std::string data_path = "C:\\Users\\taizh\\Documents\\Qt_projects\\QT_Circle_Generator\\Circle_Generator_v01\\generated_test_data\\";
    file_remover(data_path);

    std::string data_name = "Test_Circles";

    std::ofstream fw(data_path + data_name + "." + output_type, std::ofstream::out);

    // geneartes positive samples ##############################################################################################
    std::cout << "Generating circles in data" << std::endl;
    for (int i=0; i<iloops; i++){
        //variance in circle location
        int min_coord = iradius;
        int max_coord = isize-iradius;

        int x_coord = mt_rand_generator_class::mt_rand_generator(min_coord, max_coord);
        int y_coord = mt_rand_generator_class::mt_rand_generator(min_coord, max_coord);

        // labels for file
        // num_labels, num_circles with radius r_1, num_circles r_2, coord of num_circles r_1, coord of num_circles r_2

        canvas_drawer();

        noise_generator(inoise);

        circle_generator(imin_num_dots, imax_num_dots, iradius, x_coord, y_coord);

        // uncomment for check on values of final_canvas.final_list_of_coord
//        for (int k=0;k<static_cast<int>(final_canvas.final_list_of_coord.size());k++){
//            std::cout << final_canvas.final_list_of_coord[k].x << " " << final_canvas.final_list_of_coord[k].y << " ";
//        }

        //draw new partial circle, ignore pixels outside of array range
        for(int k=0;k<static_cast<int>(final_canvas.final_list_of_coord.size());k++){
            if (final_canvas.final_list_of_coord[k].x<0 or final_canvas.canvas_size<final_canvas.final_list_of_coord[k].x){
                continue;
            }
            if (final_canvas.final_list_of_coord[k].y<0 or final_canvas.canvas_size<final_canvas.final_list_of_coord[k].y){
                continue;
            }

            final_canvas.canvas_array[final_canvas.final_list_of_coord[k].x][final_canvas.final_list_of_coord[k].y]=1.0;
        }

        // uncomment for checking on content of canvas
//        for(int i=0; i<final_canvas.canvas_size; i++){
//            for(int j=0; j<final_canvas.canvas_size; j++){
//                std::cout << final_canvas.canvas_array[i][j];
//            }
//        }

        if (fw.is_open())
        {
            // store array contents to file
            std::string temp_string;

            // temporary for now
            temp_string += "1,1,";

            for (int m=0;m<final_canvas.canvas_size;m++) {
                for (int n=0;n<final_canvas.canvas_size;n++){
                    temp_string += std::to_string(final_canvas.canvas_array[m][n]);
                    temp_string += ",";
                }
                // decomment the following three comments for better visibilities of circles in test data
                //temp_string += "\n";
            }
            temp_string += "\n";
            fw << temp_string;
        }
        else std::cout << "Problem with opening file" << std::endl;

        list_of_coord.clear();
        final_canvas.final_list_of_coord.clear();


        if (i%100==0){
            std::cout << "Writing positive sample file Nr. " << i + 1 << std::endl;
        }

        free(final_canvas.canvas_array);
    }

    // generate negative samples #####################################################################
    std::cout << "Generating negative samples" << std::endl;
    for (int i=0; i<iloops; i++){
        canvas_drawer();

        noise_generator(inoise);

        for(int k=0;k<static_cast<int>(final_canvas.final_list_of_coord.size());k++){
            final_canvas.canvas_array[final_canvas.final_list_of_coord[k].x][final_canvas.final_list_of_coord[k].y]=1.0;
        }

        if (fw.is_open())
        {
            // store array contents to file
            std::string temp_string;

            // temporary for now
            temp_string += "1,0,";

            for (int m=0;m<final_canvas.canvas_size;m++) {
                for (int n=0;n<final_canvas.canvas_size;n++){
                    temp_string += std::to_string(final_canvas.canvas_array[m][n]);
                    temp_string += ",";
                }
                // decomment the following three comments for better visibilities of circles in test data
//                temp_string += "\n";
            }
            temp_string += "\n";
            fw << temp_string;
        }
        else std::cout << "Problem with opening file" << std::endl;
        list_of_coord.clear();
        final_canvas.final_list_of_coord.clear();


        if (i%100==0){
            std::cout << "Writing negative sample file Nr. " << i+1 << std::endl;
        }

        free(final_canvas.canvas_array);

//        external_file_generator(i, data_path, "Test_Circles_neg_", output_type);
    }

    fw.close();
}

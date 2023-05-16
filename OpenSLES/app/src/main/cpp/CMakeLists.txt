
cmake_minimum_required(VERSION 3.10.2)

project("opensles")

aux_source_directory(. Source)

add_library(
             native-lib

             SHARED

          ${Source})



find_library(
              log-lib

              log )


target_link_libraries(
                       native-lib
                        OpenSLES
                       ${log-lib} )
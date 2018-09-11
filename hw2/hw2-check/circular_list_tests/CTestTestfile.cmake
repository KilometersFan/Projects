# CMake generated Testfile for 
# Source directory: /home/cs104/cs104/hw-milespha/hw2/hw2-check/circular_list_tests
# Build directory: /home/cs104/cs104/hw-milespha/hw2/hw2-check/circular_list_tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ListBasic.ConstructionDestruction "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/circular_list_tests/circular_list_test" "--gtest_filter=ListBasic.ConstructionDestruction" "--gtest_color=yes")
add_test(ListBasic.OneItemAdd "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/circular_list_tests/circular_list_test" "--gtest_filter=ListBasic.OneItemAdd" "--gtest_color=yes")
add_test(ListBasic.ThreeItemAdd "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/circular_list_tests/circular_list_test" "--gtest_filter=ListBasic.ThreeItemAdd" "--gtest_color=yes")
add_test(ListBasic.Empty "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/circular_list_tests/circular_list_test" "--gtest_filter=ListBasic.Empty" "--gtest_color=yes")
add_test(ListBasic.OneItemRemove "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/circular_list_tests/circular_list_test" "--gtest_filter=ListBasic.OneItemRemove" "--gtest_color=yes")
add_test(ListBasic.ThreeItemRemove "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/circular_list_tests/circular_list_test" "--gtest_filter=ListBasic.ThreeItemRemove" "--gtest_color=yes")
add_test(ListBasic.Set "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/circular_list_tests/circular_list_test" "--gtest_filter=ListBasic.Set" "--gtest_color=yes")
add_test(ListBasic.CircularIndicies "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/circular_list_tests/circular_list_test" "--gtest_filter=ListBasic.CircularIndicies" "--gtest_color=yes")

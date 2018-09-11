# CMake generated Testfile for 
# Source directory: /home/cs104/cs104/hw-milespha/hw2/hw2-check/selection_sort_tests
# Build directory: /home/cs104/cs104/hw-milespha/hw2/hw2-check/selection_sort_tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(SelSortBasic.OneElementFindMin "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/selection_sort_tests/selection_sort_test" "--gtest_filter=SelSortBasic.OneElementFindMin" "--gtest_color=yes")
add_test(SelSortBasic.ThreeElementFindMin "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/selection_sort_tests/selection_sort_test" "--gtest_filter=SelSortBasic.ThreeElementFindMin" "--gtest_color=yes")
add_test(SelSortBasic.TenElementFindMin "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/selection_sort_tests/selection_sort_test" "--gtest_filter=SelSortBasic.TenElementFindMin" "--gtest_color=yes")
add_test(SelSortBasic.ThreeElementSort "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/selection_sort_tests/selection_sort_test" "--gtest_filter=SelSortBasic.ThreeElementSort" "--gtest_color=yes")
add_test(SelSortBasic.TenElementSort "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/selection_sort_tests/selection_sort_test" "--gtest_filter=SelSortBasic.TenElementSort" "--gtest_color=yes")

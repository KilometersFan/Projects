# CMake generated Testfile for 
# Source directory: /home/cs104/cs104/hw-milespha/hw2/hw2-check/duck_duck_goose_tests
# Build directory: /home/cs104/cs104/hw-milespha/hw2/hw2-check/duck_duck_goose_tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(DDGRoundBasic.TwoPlayers "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/duck_duck_goose_tests/duck_duck_goose_test" "--gtest_filter=DDGRoundBasic.TwoPlayers" "--gtest_color=yes")
add_test(DDGBasic.FivePlayers "/usr/bin/valgrind" "--tool=memcheck" "--leak-check=yes" "--error-exitcode=42" "--trace-children=no" "/home/cs104/cs104/hw-milespha/hw2/hw2-check/duck_duck_goose_tests/duck_duck_goose_test" "--gtest_filter=DDGBasic.FivePlayers" "--gtest_color=yes")

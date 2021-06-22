# Adder test (default):
#   % make clean
#   % make TEST=ADD build
#   % make TEST=ADD run
#
# For sub and mul, use TEST=ADD and TEST=MUL respectively

TEST=ADD

clean:
	/bin/rm -rf counter.vcd obj_dir

build:
	verilator -Wno-fatal -Wall --cc $(TEST).v --exe test.cpp -y .. --trace
	CPPFLAGS=-Dtest_$(TEST) make -j -C obj_dir/ -f V$(TEST).mk V$(TEST)

run:
	obj_dir/V$(TEST)


########################################################################
# FANCY/"CORRECT" way to do it, with dependences and all...
# 
# run: obj_dir/V$(TEST)
# 	obj_dir/V$(TEST)
# 
# obj_dir/V$(TEST): obj_dirV$(TEST).mk
# 	make -j -C obj_dir/ -f V$(TEST).mk V$(TEST)
# 
# obj_dirV$(TEST).mk: $(TEST).cpp $(TEST).v
# 	verilator -Wno-fatal -Wall --cc $(TEST).v --exe $(TEST).cpp -y .. --trace
# 
# build:
# 	verilator -Wno-fatal -Wall --cc $(TEST).v --exe $(TEST).cpp -y .. --trace
# 	make -j -C obj_dir/ -f V$(TEST).mk V$(TEST)
# 	obj_dir/V$(TEST)

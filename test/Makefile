# Adder test (default):
#   % make clean
#   % make build TEST=ADD
#   % make run   TEST=ADD
#
# Or simply "make all TEST=ADD"
# Similarly, can do "make all TEST=SUB" and "make all TEST=MUL"

# Default is ADD, can override with command line as shown above.
TEST=ADD

clean:
	/bin/rm -rf counter.vcd obj_dir

build:
	verilator -Wno-fatal -Wall --cc $(TEST).v --exe test.cpp -y .. --trace
	CPPFLAGS=-Dtest_$(TEST) make -j -C obj_dir/ -f V$(TEST).mk V$(TEST)

run:
	@obj_dir/V$(TEST)

all:
	make clean
	make build TEST=$(TEST)
	make run TEST=$(TEST)


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

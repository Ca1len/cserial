target("serial")
do
	set_kind("$(kind)")
	add_files("./src/cserial.cpp")
end
target_end()

target("cserial")
do
	set_kind("binary")
	add_files("./src/test_baud.c")
end
target_end()

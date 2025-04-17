add_requires("gtest", { system = false })

target("tests")
do
	add_deps("icserial", "udp", "serial")
	set_languages("cxx17")
	set_kind("binary")

	add_files("./gtest.cpp", "./test_cserial.cpp", "./test_udp.cpp", "./test_serial.cpp")

	add_packages("gtest")
	-- add_tests("gtests")
end
target_end()

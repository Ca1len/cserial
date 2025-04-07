add_requires("gtest", { system = false })

target("tests")
do
	add_deps("icserial")
	set_languages("cxx17")
	set_kind("binary")

	add_files("./gtest.cpp", "./test_cserial.cpp")

	add_packages("gtest")
	-- add_tests("gtests")
end
target_end()

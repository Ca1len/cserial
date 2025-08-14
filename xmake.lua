add_rules("mode.debug", "mode.release")

target("circular_buffer")
do
	set_languages("gnu99")
	set_kind("$(kind)")
	add_files("./src/circularbuffer.c")
	add_includedirs("./include/", { public = true })
end
target_end()

target("udp")
do
	set_languages("gnu99")
	set_kind("$(kind)")
	add_files("./src/udp.c")
	add_includedirs("./include/", { public = true })
end
target_end()

target("serial")
do
	set_languages("gnu99")
	set_kind("$(kind)")
	add_files("./src/serial.c")
	add_includedirs("./include/", { public = true })
end
target_end()

target("icserial")
do
	set_languages("gnu99")
	set_kind("$(kind)")
	add_files("./src/cserial.c")
	add_includedirs("./include/", { public = true })
	add_deps("serial", "udp", "circular_buffer")
end
target_end()

target("cserial")
do
	set_languages("gnu99")
	set_kind("binary")
	add_files("./src/main.c")
	add_deps("icserial")
end
target_end()

includes("./tests/")

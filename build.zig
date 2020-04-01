const std = @import("std");
const builtin = @import("builtin");

const RaylibIncludeDir = "C:/raylib/include";
const RaylibLibraryPath = "C:/raylib/lib/raylib";

const is_windows = std.Target.current.os.tag == std.Target.Os.Tag.windows;

pub fn build(b: *std.build.Builder) void {
    b.release_mode = builtin.Mode.Debug;

    const mode = b.standardReleaseOptions();

    var exe = b.addExecutable("brain", null);
    exe.setBuildMode(mode);
    exe.setOutputDir("build/");
    exe.addCSourceFile("code/brain.c", &[_][]const u8{"-std=c99"});
    exe.addCSourceFile("code/nuklear_compile.c", &[_][]const u8{ "-std=c99", "-Wno-implicit-function-declaration", "-Wno-implicit-int", "-Wno-int-conversion" });

    if (is_windows) {
        exe.linkSystemLibrary("user32");
        exe.linkSystemLibrary("gdi32");
        exe.addIncludeDir(RaylibIncludeDir);
        exe.linkSystemLibrary(RaylibLibraryPath);
    } else {
        exe.linkSystemLibrary("GL");
        exe.linkSystemLibrary("X11");
        exe.linkSystemLibrary("raylib");
    }
    exe.linkLibC();
    exe.install();

    var run_step = exe.run();
    run_step.step.dependOn(b.getInstallStep());

    //b.default_step.dependOn(&run_step.step);
}

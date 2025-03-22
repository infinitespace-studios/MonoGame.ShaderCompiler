namespace BuildScripts;

[TaskName("Build macOS")]
[IsDependentOn(typeof(PrepTask))]
[IsDependeeOf(typeof(BuildToolTask))]
public sealed class BuildMacOSTask : FrostingTask<BuildContext>
{
    public override bool ShouldRun(BuildContext context) => context.IsRunningOnMacOs();

    public override void Run(BuildContext context)
    {
        var buildWorkingDir = "dxc_build/";
        context.EnsureDirectoryExists(buildWorkingDir);
        context.StartProcess("cmake", new ProcessSettings { WorkingDirectory = buildWorkingDir, Arguments = "-DCMAKE_OSX_ARCHITECTURES=\"x86_64;arm64\" -C ../external/DirectXShaderCompiler/cmake/caches/PredefinedParams.cmake ../external/DirectXShaderCompiler/CMakeLists.txt" });
        context.StartProcess("make", new ProcessSettings { WorkingDirectory = buildWorkingDir, Arguments = "" });
        var files = Directory.GetFiles(System.IO.Path.Combine (buildWorkingDir, "bin"), "dxc", SearchOption.TopDirectoryOnly);
        context.CopyFile(files[0], $"{context.ArtifactsDir}/dxc");
        files = Directory.GetFiles(System.IO.Path.Combine (buildWorkingDir, "lib"), "*.dylib", SearchOption.TopDirectoryOnly);
        foreach (var file in files)
        {
            context.CopyFile(file, $"{context.ArtifactsDir}/{System.IO.Path.GetFileName(file)}");
        }

        buildWorkingDir = "spirv_build/";
        context.EnsureDirectoryExists(buildWorkingDir);
        context.StartProcess("cmake", new ProcessSettings { WorkingDirectory = buildWorkingDir, Arguments = "-DCMAKE_OSX_ARCHITECTURES=\"x86_64;arm64\" ../external/SPIRV-Cross/CMakeLists.txt" });
        context.StartProcess("make", new ProcessSettings { WorkingDirectory = buildWorkingDir, Arguments = "" });
        files = Directory.GetFiles(System.IO.Path.Combine (buildWorkingDir), "spirv-cross", SearchOption.TopDirectoryOnly);
        context.CopyFile(files[0], $"{context.ArtifactsDir}/spirv-cross");
    }
}
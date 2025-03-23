namespace BuildScripts;

[TaskName("Build linux")]
[IsDependentOn(typeof(PrepTask))]
[IsDependeeOf(typeof(BuildToolTask))]
public sealed class BuildLinuxTask : FrostingTask<BuildContext>
{
    public override bool ShouldRun(BuildContext context) => context.IsRunningOnLinux();

    public override void Run(BuildContext context)
    {
        var buildWorkingDir = "dxc_build/";
        context.EnsureDirectoryExists(buildWorkingDir);
        context.StartProcess("cmake", new ProcessSettings { WorkingDirectory = buildWorkingDir, Arguments = "-C ../external/DirectXShaderCompiler/cmake/caches/PredefinedParams.cmake ../external/DirectXShaderCompiler/CMakeLists.txt" });
        context.StartProcess("make", new ProcessSettings { WorkingDirectory = buildWorkingDir, Arguments = "" });
        var files = Directory.GetFiles(System.IO.Path.Combine (buildWorkingDir, "bin"), "dxc", SearchOption.TopDirectoryOnly);
        context.CopyFile(files[0], $"{context.ArtifactsDir}/dxc");

        buildWorkingDir = "spirv_build/";
        context.EnsureDirectoryExists(buildWorkingDir);
        context.StartProcess("cmake", new ProcessSettings { WorkingDirectory = buildWorkingDir, Arguments = " ../external/SPIRV-Cross/CMakeLists.txt" });
        context.StartProcess("make", new ProcessSettings { WorkingDirectory = buildWorkingDir, Arguments = "" });
        files = Directory.GetFiles(System.IO.Path.Combine (buildWorkingDir), "spirv-cross", SearchOption.TopDirectoryOnly);
        context.CopyFile(files[0], $"{context.ArtifactsDir}/spirv-cross");
    }
}
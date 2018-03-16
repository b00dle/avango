import avango
import avango.script
import avango.gua
from examples_common.GuaVE import GuaVE
import sys


class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    def evaluate(self):
        self.MatrixOut.value = avango.gua.make_trans_mat(
            0.0, -1.0, -2.0) * avango.gua.make_rot_mat(10 * self.TimeIn.value *
                                                       2.0, 0.0, 1.0, 0.0)

def set_grid_dimensions(spoints_geode, dim):
    if len(dim) == 3:
        spoints_geode.GlobalGridDimensionX.value = dim[0]
        spoints_geode.GlobalGridDimensionY.value = dim[1]
        spoints_geode.GlobalGridDimensionZ.value = dim[2]

def set_point_precision(spoints_geode, prec):
    if len(prec) == 3:
        spoints_geode.GlobalPointPrecisionX.value = prec[0]
        spoints_geode.GlobalPointPrecisionY.value = prec[1]
        spoints_geode.GlobalPointPrecisionZ.value = prec[2]

def set_compression_settings(spoints_geode, settings):
    if "point_precision" in settings and "grid_dimension" in settings:
        set_grid_dimensions(spoints_geode, settings["grid_dimension"]) 
        set_point_precision(spoints_geode, settings["point_precision"])

def start(filename):
    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    compression_settings = {
        "point_precision" : [4,4,4],
        "grid_dimension" : [2,2,2]
    }

    spointsloader = avango.gua.nodes.SPointsLoader()
    spoints_geode = spointsloader.load("kinect", filename)
    set_compression_settings(spoints_geode, compression_settings)

    transform1 = avango.gua.nodes.TransformNode(Children=[spoints_geode])

    light = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name="light",
        Color=avango.gua.Color(1.0, 1.0, 1.0),
        Brightness=100.0,
        Transform=(avango.gua.make_trans_mat(1, 1, 5) *
                   avango.gua.make_scale_mat(30, 30, 30)))

    size = avango.gua.Vec2ui(1024, 768)

    window = avango.gua.nodes.GlfwWindow(Size=size, LeftResolution=size)

    avango.gua.register_window("window", window)

    cam = avango.gua.nodes.CameraNode(
        LeftScreenPath="/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5))

    res_pass = avango.gua.nodes.ResolvePassDescription()
    res_pass.EnableSSAO.value = True
    res_pass.SSAOIntensity.value = 4.0
    res_pass.SSAOFalloff.value = 10.0
    res_pass.SSAORadius.value = 7.0

    #res_pass.EnableScreenSpaceShadow.value = True

    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)

    anti_aliasing = avango.gua.nodes.SSAAPassDescription()

    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            avango.gua.nodes.SPointsPassDescription(),
            res_pass,
            anti_aliasing,
        ])

    cam.PipelineDescription.value = pipeline_description

    screen = avango.gua.nodes.ScreenNode(
        Name="screen",
        Width=2,
        Height=1.5,
        Children=[cam])

    graph.Root.value.Children.value = [transform1, light, screen]

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    monkey_updater = TimedRotate()

    timer = avango.nodes.TimeSensor()
    monkey_updater.TimeIn.connect_from(timer.Time)

    transform1.Transform.connect_from(monkey_updater.MatrixOut)

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
    start(sys.argv[1])

--
-- Test scene with all supported features
-- 
scene {
    lights={
        pointlight{
            radiance={r=100.0, g=100.0, b=100.0},
            location={x=0.0, y=10.0, z=-10.0},
        },
    }, 
    children={
        transform{
            translation={x=2.0, y=2.0, z=0.0},
            rotation={
                quat{angle=math.pi/4.0, x=0.0, y=1.0, z=0.0},
                quat{angle=-math.pi/6.0, x=0.0, y=0.0, z=1.0},
            },
            child=trimesh{
                    filename="../data/teapot.obj",
                    material=lambertian{r=1.0, g=0.0, b=0.0}} 
        },
        plane{
            pt={x=0.0, y=-10.0, z=0.0},
            norm={x=0.0, y=1.0, z=0.0}, 
            material=lambertian{r=0.0, g=1.0, b=0.0}
        },
        sphere{
            centre={x=-5.0, y=0.0, z=2.0},
            radius=2.0,
            material=lambertian{r=0.0, g=0.0, b=1.0}
        }, 
    }
}

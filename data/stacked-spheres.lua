--
-- Test scene for path tracing
--
scene {
    r=0,
    g=0,
    b=0,
    children={
        sphere{
            centre={x=0, y=-1000, z=0},
            radius=1000.0,
            material=lambertian{r=0.2, g=0.2, b=0.2, reflectivity=0.2}
        },
        sphere{
            centre={x=0.0, y=2.0, z=0.0},
            radius=2.0,
            material=lambertian{r=0.5, g=0.0, b=0.0, reflectivity=1.0}
        },
        sphere{
            centre={x=0.0, y=6.0, z=0.0},
            radius=2.0,
            material=diffuse{r=5.0, g=5.0, b=5.0}
        },
    }
}

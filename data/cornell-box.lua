scene {
    r=0,
    g=0,
    b=0,
    children={
        --top
        plane{
            pt={x=0.0, y=10.0, z=0.0},
            norm={x=0.0, y=-1.0, z=0.0},
            material=lambertian{r=1.0, g=1.0, b=1.0, reflectivity=0.2}
        },
        --left
        plane{
            pt={x=-5.0, y=0.0, z=0.0},
            norm={x=1.0, y=0.0, z=0.0},
            material=lambertian{r=1.0, g=0.0, b=0.0, reflectivity=0.2}
        },
        --right
        plane{
            pt={x=5.0, y=0.0, z=0.0},
            norm={x=-1.0, y=0.0, z=0.0},
            material=lambertian{r=0.0, g=1.0, b=0.0, reflectivity=0.2}
        },
        --back
        plane{
            pt={x=0.0, y=0.0, z=-10.0},
            norm={x=0.0, y=0.0, z=1.0},
            material=lambertian{r=1.0, g=1.0, b=1.0, reflectivity=0.2}
        },
        --bottom
        plane{
            pt={x=0.0, y=0.0, z=0.0},
            norm={x=0.0, y=1.0, z=0.0},
            material=lambertian{r=1.0, g=1.0, b=1.0, reflectivity=0.2}
        },
        sphere{
            centre={x=-2.75, y=2.0, z=-5.25},
            radius=1.5,
            material=specular{}
        },
        sphere{
            centre={x=2.0, y=2.0, z=-4.5},
            radius=1.5,
            material=dielectric{nt=1.6}
        },
        trimesh{
            vertices={{x=-0.2, y=9.9, z=-4.8},
                      {x=0.2, y=9.9, z=-4.8},
                      {x=0.2, y=9.9, z=-5.2},
                      {x=-0.2, y=9.9, z=-5.2}},
            faces={{i=0, j=1, k=2},
                   {i=0, j=2, k=3}},
            material=diffuse{r=100.0, g=100.0, b=100.0}
        },
    }
}

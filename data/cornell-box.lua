
scene { 
    lights={
        rectangularlight{
            radiance={r=20.0, g=20.0, b=20.0},
            pt1={x=-0.4, y=9.9, z=-4.6},
            pt2={x=0.4, y=9.9, z=-5.4},
        },
    }, 
    children={
        --top
        plane{
            pt={x=0.0, y=10.0, z=0.0},
            norm={x=0.0, y=-1.0, z=0.0}, 
            material=lambertian{r=1.0, g=1.0, b=1.0}
        },
        --left
        plane{
            pt={x=-5.0, y=0.0, z=0.0},
            norm={x=1.0, y=0.0, z=0.0}, 
            material=lambertian{r=1.0, g=0.0, b=0.0}
        },
        --right
        plane{
            pt={x=5.0, y=0.0, z=0.0},
            norm={x=-1.0, y=0.0, z=0.0},
            material=lambertian{r=0.0, g=1.0, b=0.0}
        },
        --back
        plane{
            pt={x=0.0, y=0.0, z=-10.0},
            norm={x=0.0, y=0.0, z=1.0},
            material=lambertian{r=1.0, g=1.0, b=1.0}
        },
        --bottom
        plane{
            pt={x=0.0, y=0.0, z=0.0},
            norm={x=0.0, y=1.0, z=0.0},
            material=lambertian{r=1.0, g=1.0, b=1.0}
        },
        sphere{
            centre={x=-2.75, y=2.0, z=-5.25},
            radius=1.5,
            material=specular{}
        }, 
        sphere{
            centre={x=2.0, y=2.0, z=-4.5},
            radius=1.5,
            material=dielectric{nt=1.2}
        }, 
    } 
}


scene { 
    lights={
        pointlight{
            location={x=0.0, y=9.9, z=-5.0},
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
            centre={x=-1.5, y=2.0, z=-5.0},
            radius=1.5,
            material=lambertian{r=0.0, g=1.0, b=0.0}
        }, 
        sphere{
            centre={x=2.0, y=2.0, z=-4.0},
            radius=1.5,
            material=lambertian{r=0.0, g=0.0, b=1.0}
        }, 
    } 
}

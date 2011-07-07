
scene { 
    lights={
        pointlight{
            location={x=0.0, y=9.0, z=-6.5},
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
        }
    } 
}

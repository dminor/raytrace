--
-- Random sphere scene from Peter Shirley's Ray Tracing in a Weekend
--
spheres = {}
spheres[1] = sphere{
    centre={x=0, y=-1000, z=0},
    radius=1000.0,
    material=lambertian{r=0.5, g=0.5, b=0.5, reflectivity=1.0}
}

i = 2
for a=-11,11 do
    for b=-11,11 do
        spheres[i] = sphere{
            centre={x=a + 0.9*math.random(), y=0.2, z=b + 0.9*math.random()},
            radius=0.2,
            material=lambertian{r=math.random()*math.random(),
                                g=math.random()*math.random(),
                                b=math.random()*math.random(),
                                reflectivity=1.0}
        }
        i = i + 1
    end
end

spheres[i] = sphere{
    centre={x=0.0, y=1.0, z=0.0},
    radius=1.0,
    material=dielectric{nt=1.5}
}
i = i + 1

spheres[i] = sphere{
    centre={x=-4.0, y=1.0, z=0.0},
    radius=1.0,
    material=lambertian{r=0.4, g=0.2, b=0.1, reflectivity=1.0}
}
i = i + 1

spheres[i] = sphere{
    centre={x=4, y=1, z=0},
    radius=1.0,
    material=specular{}
}

scene {
    lights={
        pointlight{
            radiance={r=100000.0, g=100000.0, b=100000.0},
            location={x=0.0, y=10.0, z=-10.0},
        },
    },
    children=spheres
}

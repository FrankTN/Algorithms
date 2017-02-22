

int Raytracer::parseRenderMode(const YAML::Node& node){
    int result;
    if(node[0] == "zbuffer"){
        return 1;
    }else{
        return 0;
    }
}

//Inside the parseScene method:

scene->setRenderMode(parseRenderMode(doc["RenderMode"]));

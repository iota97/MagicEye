using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class exporter : MonoBehaviour {
    void Start() {
        string ret = "\n\tObject obj;\n";
        List<string> Models = new List<string>();
        List<string> Textures = new List<string>();

        foreach (Transform child in transform) {
            string mesh = AssetDatabase.GetAssetPath(child.GetComponent<MeshFilter>().sharedMesh);
            mesh = mesh.Substring(7);
            int mesh_idx = -1;
            for (int i = 0; i < Models.Count; i++) {     
                if (Models[i] == mesh) {
                    mesh_idx = i;
                    break;
                }           
            }
            if (mesh_idx == -1) {
                mesh_idx = Models.Count;
                Models.Add(mesh);
            }

            string tex = AssetDatabase.GetAssetPath(child.GetComponent<Renderer>().material.mainTexture);
            tex = tex.Substring(7);
            int tex_idx = -1;
            for (int i = 0; i < Textures.Count; i++) {     
                if (Textures[i] == tex) {
                    tex_idx = i;
                    break;
                }           
            }
            if (tex_idx == -1) {
                tex_idx = Textures.Count;
                Textures.Add(tex);
            }

            ret += "\tobj.model = &models["+mesh_idx+"];\n";
            ret += "\tobj.texture = textures["+tex_idx+"];\n";
            ret += "\tobj.position = glm::vec3"+child.transform.position+";\n";
            ret += "\tobj.rotation = glm::vec3"+child.transform.eulerAngles+";\n";
            ret += "\tobj.scale = glm::vec3"+child.transform.lossyScale+";\n";
            ret += "\tobj.textureRepeat = "+child.GetComponent<Renderer>().material.GetTextureScale("_MainTex").x+";\n";
            ret += "\tobjects.push_back(obj);\n";
        }

        Models.Reverse();
        Textures.Reverse();
        foreach (string path in Models) {
            ret = "\tmodels.push_back(Model(\"assets/models/"+ path + "\"));\n" + ret;
        }
        foreach (string path in Textures) {
            ret = "\ttextures.push_back(LoadTexture(\"assets/textures/"+ path + "\"));\n" + ret;
        }
        
        System.IO.File.WriteAllText("scene.txt", ret);
        UnityEditor.EditorApplication.isPlaying = false;
    }
}
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;

public class follow_headset : MonoBehaviour
{
    public GameObject centereyeanchor;

    private Transform centereyeanchor_tf;
    private Transform tf;

    // Start is called before the first frame update
    void Start()
    {
        centereyeanchor_tf = centereyeanchor.GetComponent<Transform>();
        tf = GetComponent<Transform>();
    }

    // Update is called once per frame
    void Update()
    {
        //need to convert between quaternion and vector3
        Vector3 eye_rot = centereyeanchor_tf.rotation.eulerAngles;
        tf.rotation = Quaternion.Euler(0,eye_rot.y,0); //new Vector3(0,eye_rot.y,0);        
    }
}

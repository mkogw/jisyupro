using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;

public class follow_headset_delayed : MonoBehaviour
{
    public GameObject centereyeanchor;
    public float delay_time;

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
        StartCoroutine(DelayMethod(delay_time, () =>
        {
            tf.rotation = Quaternion.Euler(0,eye_rot.y,0); //new Vector3(0,eye_rot.y,0);    
        }));
            
    }

    private IEnumerator DelayMethod(float waitTime, Action action)
    {
        yield return new WaitForSeconds(waitTime);
        action();
    }
}


/*
private void Start()
{
    //3.5秒後に実行する
    StartCoroutine(DelayMethod(3.5f, () =>
    {
        Debug.Log("Delay call");
    }));
}

/// <summary>
/// 渡された処理を指定時間後に実行する
/// </summary>
/// <param name="waitTime">遅延時間[ミリ秒]</param>
/// <param name="action">実行したい処理</param>
/// <returns></returns>
private IEnumerator DelayMethod(float waitTime, Action action)
{
    yield return new WaitForSeconds(waitTime);
    action();
}
*/
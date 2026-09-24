using UnityEngine;

public class WhilePressControl : MonoBehavior
{
    public float power = 0f;
    public float changedS = 5f;

    void Update()
    {
        if(Input.GetKey(KeyCode.W))
        {
            power += changedS * Time.deltaTime; 
        }

		    if(Input,GetKey(KeyCode.S))
		    {
  			    power -= changedS * Time.deltaTime; 
		    }
    }
}

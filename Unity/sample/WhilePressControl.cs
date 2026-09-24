using UnityEngine;

public class WhilePressControl : MonoBehavior
{
    public float power = 0f;
    public float changedS = 5f;
	public float returnSpeed = 3f;

    void Update()
    {
        if(Input.GetKey(KeyCode.W))
        {
            power += changedS * Time.deltaTime; 
        }

		if(Input,GetKey(KeyCode.S))
		{
			power -= c
			hangedS * Time.deltaTime; 
		}

		else {

			power = Mathf.MoveTowards(power, 0f, returnSpeed * Time.deltaTime);
		}
    }
}

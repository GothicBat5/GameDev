using UnityEngine;
using TMPro;

public class UIWhilePressControl : MonoBehavior
{
    public float power = 0f;
    public float increaseSpd = 5f;
	  public float returnSpeed = 3f;
	  public TMP_Text pTx;

    void Update()
    {
		  if(Input.GetKey(KeyCode.W))
		  {
			    power += increaseSpd * Time.deltaTime;	
		  }

		  else if(Input.GetKey(KeyCode.S))
		  {
			    power -= increaseSpd * Time.deltaTime;
		  }

		  else {
			    power = Mathf.MoveTowards(power, 0f, returnSpeed * Time.deltaTime);
		  }

		  pTx.text = $"Power: {power:F1}";
    }
}

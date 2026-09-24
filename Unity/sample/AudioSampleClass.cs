using UnityEngine;

public class AudioSampleClass : MonoBehavior
{
	public AudioClip soundsample1;
	public AudioClip soundsample2;
	public AudioClip soundsample3;
	public AudioClip soundsample4;
	public AudioClip soundsample5;

    void Update()
    {
		    if(Input.GetKeyDown(KeyCode.A))
		    {
			      audioSource.PlayOneShot(soundsample1);
		    }

		    if(Input.GetKeyDown(KeyCode.S))
		    {
			      audioSource.PlayOneShot(soundsample2);
		    }

		    if(Input.GetKeyDown(KeyCode.D))
		    {
			      audioSource.PlayOneShot(soundsample3);
		    }

		    if(Input.GetKeyDown(KeyCode.W))
		    {
			      audioSource.PlayOneShot(soundsample4);
		    }

		    if(Input.GetKeyDown(KeyCode.Q))
		    {
			      audioSource.PlayOneShot(soundsample5);
		    }
        
    }
}

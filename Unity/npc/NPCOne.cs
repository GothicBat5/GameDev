using UnityEngine;

public class NPCOne : MonoBehavior
{
    public float speed = 3f;
    
    void Update()
    {
        transform.Translate(Vector3.forward * speed * Time.deltaTime);
        
        Ray ray = new Ray(transform.position, transform.forward);
        RaycastHit hit;
        
        if(Physics.Raycast(ray, out hit, 1.5f))
        {
            float angle = Random.Range(90f, 270f);
            transform.Rotate(0, angle, 0);
        }
        
        Debug.DrawRay(transform.position, transform.forward * 1.5f, Color.red);
    }
    
    void OnCollisionEnter()
    {
        Debug.log(" Hit !");
        
        float angle = Random.Range(90f, 270f);
        transform.Rotate(0, angle, 0);
    }
}

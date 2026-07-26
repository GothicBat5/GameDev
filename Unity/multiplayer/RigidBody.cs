Rigidbody rb;

void FixedUpdate()
{
    if (!IsOwner) return;

    float h = Input.GetAxis("Horizontal");
    float v = Input.GetAxis("Vertical");

    rb.MovePosition(rb.position + new Vector3(h, 0, v) * speed * Time.fixedDeltaTime);
}

float ClampedBrighten(float value, float brightness) {
    return (value + brightness) / (1.0f + brightness);
}

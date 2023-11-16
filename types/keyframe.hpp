// Base class for a keyframe
class KeyframeBase {
public:
    float time; // The time at which the keyframe is set

    KeyframeBase(float t) : time(t) {}
    virtual ~KeyframeBase() {}

    // Virtual function for applying the keyframe value to the shader
    virtual void applyToShader(/* Shader reference or ID */) = 0;
};

// Template class for a keyframe with a specific type
template <typename T>
class Keyframe : public KeyframeBase {
public:
    T value; // The value of the keyframe

    Keyframe(float t, T val) : KeyframeBase(t), value(val) {}

    void applyToShader(/* Shader reference or ID */) override {
        // Implementation for applying 'value' to the shader at 'time'
    }
};

// You can add more specific implementations or specializations if needed

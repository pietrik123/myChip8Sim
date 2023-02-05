
template <class T>
class ChipInterface
{
public:
    virtual T fetchInstruction() = 0;
    virtual void execute(T instruction) = 0;
};
modded class ActionInput
{
    protected bool m_ZenLazyContinuous;

    bool IsZenLazyActive()
    {
        return m_ZenLazyContinuous;
    }

    void SetZenLazyActive(bool b)
    {
        m_ZenLazyContinuous = b;
    }

    override void Update()
    {
        #ifdef ZENMODPACK 
        if (!ZenModEnabled("ZenLazyActions"))
        {
            super.Update();
            return;
        }
        #endif

        if (!m_Enabled)
        {
            m_Active = false;
            m_JustActivate = false;
            m_ZenLazyContinuous = false;
            return;
        }

        // Catch ANY input mapped in current profile to interrupt lazy action
        if (m_ZenLazyContinuous)
        {
            TIntArray activeInputs = new TIntArray();
            GetUApi().GetActiveInputs(activeInputs);

            for (int i = 0; i < activeInputs.Count(); i++)
            {
                UAInput input = GetUApi().GetInputByID(activeInputs[i]);
                
                // Skip actual action input and only monitor alternative inputs, or else this won't work
                if (input == m_input.InputP() || input == GetUApi().GetInputByName("UAZenLazyAction"))
                    continue;

                // Loop bindings to filter out mouse axes (ie. X/Y look)
                bool isMouseInput = false;
                int bindCount = input.BindKeyCount();
                for (int b = 0; b < bindCount; b++)
                {
                    if (input.CheckBindDevice(b, EUAINPUT_DEVICE_MOUSE))
                    {
                        isMouseInput = true;
                        break;
                    }
                }

                // Skip mouse movement inputs
                if (isMouseInput)
                    continue;

                /*/! TODO: Only allow movement commands to interrupt action if this is not a full-body locked action (ie. allow eating etc)
                if (!GetAction().IsFullBody(m_Player))
                {
                    if (input.NameHash() == GetUApi().GetInputByName("UAMoveForward").NameHash())
                        continue;
                    if (input.NameHash() == GetUApi().GetInputByName("UAMoveBack").NameHash())
                        continue;
                    if (input.NameHash() == GetUApi().GetInputByName("UAMoveLeft").NameHash())
                        continue;
                    if (input.NameHash() == GetUApi().GetInputByName("UAMoveRight").NameHash())
                        continue;
                }*/

                if (input && (input.LocalPress() || input.LocalClick()))
                {
                    m_ZenLazyContinuous = false;
                    break;
                }
            }
        }

        switch (m_InputType)
        {
            case ActionInputType.AIT_CONTINUOUS:
                m_JustActivate = false;

                if (m_Active)
                {
                    if (m_ZenLazyContinuous)
                    {
                        // Continue action even if button is not held
                        m_Active = true;
                    }
                    else
                    {
                        m_Active = m_input.InputP().LocalHold();
                    }
                }
                else
                {
                    // Action is just starting
                    m_Active = m_input.InputP().LocalHoldBegin();
                    m_JustActivate = m_Active;

                    if (m_Active && GetGame().GetInput().LocalHold("UAZenLazyAction", false))
                    {
                        // Lazy action key was held at start of action - cruise control engage
                        m_ZenLazyContinuous = true;
                    }
                    else
                    {
                        m_ZenLazyContinuous = false;
                    }
                }
                break;

            default:
                super.Update(); // let other input types handle normally
                break;
        }
    }

    override void OnActionEnd()
    {
        super.OnActionEnd();

        m_ZenLazyContinuous = false;
    }
}
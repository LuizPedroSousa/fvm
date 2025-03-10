/**
 * Generate a callback that is safe to be called even if `callbackObjectGetter()` is nullish on that moment.
 *
 * In the case of the object is nullish, invoking the callback will do nothing.
 */
const carefullCallbackGenerator =
  <CallbacksObject extends object, Callback extends keyof CallbacksObject>(
    callbackObjectGetter: () => CallbacksObject | null | undefined,
    callbackName: Callback
  ) =>
  (
    value: Parameters<
      CallbacksObject[Callback] extends (value: unknown) => void
        ? CallbacksObject[Callback]
        : never
    >[0]
  ) => {
    const callbackObject = callbackObjectGetter();
    if (callbackObject !== null) {
      (callbackObject as Record<Callback, (v: typeof value) => void>)[
        callbackName
      ](value);
    }
  };

/**
 * This is an source of callbacks that are safe to be called even if the object of `callbackObjectGetter()` is nullish on that moment.
 *
 * In the case of the object is nullish, invoking the callbacks will do nothing.
 */
export const carefullCallbackSource = <CallbacksObject extends object>(
  callbackObjectGetter: () => CallbacksObject | undefined
): (<Callback extends keyof CallbacksObject>(
  callbackName: Callback
) => CallbacksObject[Callback]) =>
  //@ts-expect-error unassignable
  carefullCallbackGenerator.bind(null, callbackObjectGetter);

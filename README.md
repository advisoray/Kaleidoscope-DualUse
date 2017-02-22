# Kaleidoscope-DualUse

![status][st:stable] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-DualUse.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-DualUse

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

There are a number of keys on our keyboards that we usually hold in combination
with others. We also have keys that we usually press and release, and don't want
to hold them for repeating. Then we have the others which we sometimes hold to
repeat them, such as `Space` held to insert a number of whitespace characters.
The interesting thing that falls out of this, is that there are keys we hold,
but never tap, and others which we never hold. What if we could combine these?

What if we had a key that acted as `Control` when held, but `Esc` when tapped in
isolation? No useful functionality lost, and we have one key less to place on
the keyboard! We can even put `Esc` to a more convenient position, perhaps.

Dual-use keys do just this: if you hold them, and press any other key, they will
act as a modifier or momentary layer switcher. If you hold them for a longer
period, they - again - will act as modifiers / momentary switchers. But if you
tap and release them in isolation, they will act as another key instead.

## Using the plugin

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-DualUse.h>

// in the keymap:
CTL_T(Esc), LT(_LAYER, Esc)

void setup (void) {
  Kaleidoscope.setup (KEYMAP_SIZE);
  
  Kaleidoscope.use (&DualUse, NULL);
}
```

## Keymap markup

The plugin provides a number of macros one can use in keymap definitions:

### `CTL_T(key)`

> A key that acts as the *left* `Control` when held, or used in conjunction with
> other keys, but as `key` when tapped in isolation. The `key` argument must be
> a plain old key, and can't have any modifiers or anything else applied.

### `ALT_T(key)`

> A key that acts as the *left* `Alt` when held, or used in conjunction with
> other keys, but as `key` when tapped in isolation. The `key` argument must be
> a plain old key, and can't have any modifiers or anything else applied.
    
### `SFT_T(key)`

> A key that acts as the *left* `Shift` when held, or used in conjunction with
> other keys, but as `key` when tapped in isolation. The `key` argument must be
> a plain old key, and can't have any modifiers or anything else applied.

### `GUI_T(key)`

> A key that acts as the *left* `GUI` when held, or used in conjunction with
> other keys, but as `key` when tapped in isolation. The `key` argument must be
> a plain old key, and can't have any modifiers or anything else applied.

### `MT(mod, key)`

> A key that acts as `mod` when held, or used in conjunction with other keys,
> but as `key` when tapped in isolation. The `key` argument must be a plain old
> key, and can't have any modifiers or anything else applied. The `mod` argument
> can be any of the modifiers, *left* or *right* alike.

### `LT(layer, key)`

> A key that momentarily switches to `layer` when held, or used in conjunction
> with other keys, but as `key` when tapped in isolation. The `key` argument
> must be a plain old key, and can't have any modifiers or anything else
> applied.

## Plugin methods

The plugin provides a single object, `DualUse`, with the following methods:

### `.configure(offAction)`

> Configures the plugin by telling it what to do when turned off. It can either
> make the key act as a modifier or layer switcher, or as the key component in
> the keymap declaration. The value of `offAction` can either be *zero*, where
> in the turned off state, the plugin will make the key act as the modifier or
> layer switcher; or it can be *one*, in which case, the plugin will simulate
> the `key` component.
>
> The default is to use the `key` component.

### `.on()`

> Turns the plugin on, if it has been turned off.

### `.off()`

> Turns the plugin off, by making the dual-use keys act as either the modifier
> or layer switch keys, or as the key component.

### `.timeOut`

> The number of milliseconds to wait before considering a held key in isolation
> as its secondary role. That is, we'd have to hold a `Z/Control` key this long,
> by itself, to trigger the `Control` role.
>
> Not strictly a method, it is a variable one can assign a new value to.
>
> Defaults to 1000.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-DualUse/blob/master/examples/DualUse/DualUse.ino
